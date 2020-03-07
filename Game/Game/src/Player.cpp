
#include "Player.hpp"

Player::Player(Array<int32> _dropCells1LoopNum) : dropCells1LoopNum(_dropCells1LoopNum)
{
	for (auto i : step((int32)dropCells1LoopNum.size()))
	{
		for (auto j : step(dropCells1LoopNum.at(i)))
		{
			dropCells1LoopCells << Cell(i);
		}
	}


	getDropCell(10);
}

void Player::update(PlayerKeySet keySet)
{
	const double deltaTime = Scene::DeltaTime();

	// 常に10個以上確保しておく（Next表示対策）
	getDropCell(10);

	// 操作可能
	if (canOperate)
	{
		// フィールドをランダムに変更する
		if (canDrop && keySet.toRandom.down())
		{
			field = CellField::RandomField(fieldSize, dropCells1LoopNum.size() - 1, false, false);
			updatedField();
		}

		// フィールドを無に変更する
		if (canDrop && keySet.toEmpty.down())
		{
			field = CellField(fieldSize);
			updatedField();
		}

		// セルをホールドする
		if (keySet.hold.down())
		{
			if (holdCell.getNumber() == (int32)CellTypeNumber::Empty)
			{
				holdCell = getDropCell(0);
				dropCells.remove_at(0);
			}
			else
			{
				Cell tmp = holdCell;
				holdCell = dropCells.at(0);
				dropCells.at(0) = tmp;
			}

			if (debugPrint)	Print << U"Hold...";
		}

		// 落とすセルを変更する
		if (keySet.changeCell.down())
		{
			// 1 ~ cellMaxNumberで1周する
			int32 nextNumber = getDropCell(0).getNumber() + 1;
			if (nextNumber >= dropCells1LoopNum.size())
				nextNumber = 1;
			getDropCell(0) = Cell(nextNumber);
		}

		// 落とすセルを移動する
		if (keySet.moveL.pressed() || keySet.moveR.pressed())
		{
			dropCellTimer += deltaTime;
		}
		if (keySet.moveL.pressed() && dropCellFieldX > 0)
		{
			if (dropCellTimer > dropCellCoolTime)
			{
				dropCellFieldX--;
				dropCellTimer = 0.0;
			}
		}
		if (keySet.moveR.pressed() && dropCellFieldX < fieldSize.x - 1)
		{
			if (dropCellTimer > dropCellCoolTime)
			{
				dropCellFieldX++;
				dropCellTimer = 0.0;
			}
		}
		if (keySet.moveL.up() || keySet.moveR.up())
		{
			dropCellTimer = dropCellCoolTime;
		}

		//セルを落下させる
		if (canDrop && keySet.drop.pressed())
		{
			field.pushCell(getDropCell(0), dropCellFieldX);

			// 落下処理へ移行
			canDrop = false;
			isFallingTime = true;
			fieldMoveTo = field.getFallTo();
			if (debugPrint)	Print << U"Falling...";

			dropCells.remove_at(0);
		}
	}

	// 演出処理
	{
		// セルが消える演出
		if (isDeletingTime)
		{
			deletingTimer += deltaTime;

			if (deletingTimer > deletingCoolTime)
			{
				int32 dc = field.deleteCells(just10Times);
				if (debugPrint)	Print << U"Deleted {} Cells!"_fmt(dc);
				m_score += dc;

				isDeletingTime = false;
				deletingTimer = 0.0;

				// 落下処理へ移行
				isFallingTime = true;
				fieldMoveTo = field.getFallTo();
				if (debugPrint)	Print << U"Falling...";
			}
		}

		// セルが落ちる演出
		if (isFallingTime)
		{
			fallingTimer += deltaTime;

			if (fallingTimer > fallingCoolTime)
			{
				field.moveCells(fieldMoveTo);
				if (debugPrint)	Print << U"Falled Cells!";

				isFallingTime = false;
				fallingTimer = 0.0;

				// 消えるものがあるなら、消える演出へ
				if (!updatedField())
				{
					canDrop = true;
				}
			}
		}

		// 負けと表示する演出
		if (state == (int32)BattleState::lose)
		{
			loseTimer += deltaTime;
		}
	}
}

void Player::draw(Point fieldPos, Size cellDrawSize) const
{
	// スコアの表示
	FontAsset(U"Text")(U"Score:{}"_fmt(m_score))
		.drawAt(fieldPos.movedBy(fieldSize.x * cellDrawSize.x / 2, fieldSize.y * cellDrawSize.y + 30), ColorF(0.25));

	// Nextセルの描画
	FontAsset(U"Text")(U"Next").drawAt(fieldPos + Vec2(fieldSize.x + 2, -0.5) * cellDrawSize);
	getDropCellConst(1).getTexture().resized(cellDrawSize * 2).draw(fieldPos + Vec2(fieldSize.x + 1, 0) * cellDrawSize);
	for (auto i : step(2, 4))
	{
		getDropCellConst(i).getTexture().resized(cellDrawSize).draw(fieldPos + Vec2(fieldSize.x + 1, i) * cellDrawSize);
	}

	// ホールドの表示
	FontAsset(U"Text")(U"Hold").drawAt(fieldPos + Vec2(-2, -0.5) * cellDrawSize);
	holdCell.getTexture().resized(cellDrawSize * 2).draw(fieldPos + Point(-3, 0) * cellDrawSize);

	// フィールドの背景
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).draw(ColorF(0.2, 0.8, 0.4));
	// フィールドの枠
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).drawFrame(10, Palette::Forestgreen);

	// Just10の回数の表示
	if (KeyControl.pressed())
	{
		for (auto p : step(fieldSize))
		{
			Point pos = fieldPos + cellDrawSize * p + cellDrawSize / 2;
			FontAsset(U"Text")(U"{}"_fmt(just10Times.at(p))).draw(pos, Palette::Black);
		}
	}
	// フィールドの描画
	else
	{
		field.draw(fieldPos, cellDrawSize,
			[&](Point p, int32) {
				if (just10Times.at(p))
					return (p * cellDrawSize);
				else
				{
					const double e = EaseOutCubic(fallingTimer / fallingCoolTime);
					return (Vec2(p * cellDrawSize).lerp(Vec2(fieldMoveTo.at(p) * cellDrawSize), e)).asPoint();
				}
			},
			[&](Point p, int32) {
				if (just10Times.at(p))
				{
					const double e = EaseOutCubic(deletingTimer / deletingCoolTime);
					return Color(255, (int32)(255 * (1.0 - e)));
				}
				else
					return Color(255, 255);
			},
				[this](Point, int32) {
				return Color(0, 0);
			});
	}

	// 落とすセルの描画
	{
		const Point dropCellPos(fieldPos + Point(cellDrawSize.x * dropCellFieldX, -cellDrawSize.y));

		getDropCellConst(0).getTexture().resized(cellDrawSize).draw(dropCellPos);
	}

	// 負けという知らせの描画
	if (state == (int32)BattleState::lose)
	{
		const double e = EaseOutBounce(loseTimer / loseCoolTime <= 1.0 ? loseTimer / loseCoolTime : 1.0);
		const Vec2 to(fieldPos + fieldSize * cellDrawSize / 2.0);
		const Vec2 pos(Vec2(fieldPos.x + fieldSize.x * cellDrawSize.x / 2.0, -100).lerp(to, e));
		FontAsset(U"Header")(U"Lose").drawAt(pos, Palette::Black);
	}
}

Cell& Player::getDropCell(int32 num)
{
	// dropCellsを追加
	while (num >= dropCells.size())
	{
		dropCells.append(dropCells1LoopCells.shuffled());
	}

	return dropCells.at(num);
}

const Cell& Player::getDropCellConst(int32 num) const
{
	return dropCells.at(num);
}

bool Player::updatedField()
{
	just10Times = field.getJust10Times();

	// Just10で消えるものがあるなら、消える時間にする
	if (just10Times.any([](int32 n) { return n != 0; }))
	{
		canDrop = false;
		isDeletingTime = true;
		if (debugPrint)	Print << U"Deleting...";

		return true;
	}

	// 一番上のyにセルがあるなら、負け
	for (auto x : step(field.size().x))
	{
		if (field.getGrid().at(0, x).getNumber() != (int32)CellTypeNumber::Empty)
		{
			canOperate = false;
			state = (int32)BattleState::lose;
			return false;
		}
	}

	return false;
}
