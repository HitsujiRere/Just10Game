
#include "Player.hpp"

Player::Player(Array<int32> _dropCells1LoopNum)
	: dropCells1LoopNum(_dropCells1LoopNum)
{
	for (auto i : step((int32)dropCells1LoopNum.size()))
	{
		CellType type = static_cast<CellType>(i);
		for (auto j : step(dropCells1LoopNum.at(i)))
		{
			dropCells1LoopCells << Cell(type);
		}
	}

	makeDropCells(10);
}

Player& Player::operator=(const Player& another)
{
	this->dropCells1LoopNum = another.dropCells1LoopNum;
	this->dropCells1LoopCells = another.dropCells1LoopCells;
	this->field = another.field;
	this->deleteField = another.deleteField;
	this->fieldMoveTo = another.fieldMoveTo;
	this->fieldColor = another.fieldColor;
	this->dropCellTimer = another.dropCellTimer;
	this->dropCellFieldX = another.dropCellFieldX;
	this->dropCells1LoopNum = another.dropCells1LoopNum;
	this->holdCell = another.holdCell;
	this->canOperate = another.canOperate;
	this->canDrop = another.canDrop;
	this->isDeletingTime = another.isDeletingTime;
	this->deletingTimer = another.deletingTimer;
	this->isFallingTime = another.isFallingTime;
	this->fallingTimer = another.fallingTimer;
	this->loseTimer = another.loseTimer;
	this->score = another.score;
	this->combo = another.combo;
	this->state = another.state;

	return *this;
}

int32 Player::update(PlayerKeySet keySet)
{
	int32 ret = 0;

	const double deltaTime = Scene::DeltaTime();

	// 常に10個以上確保しておく（Next表示対策）
	makeDropCells(10);

	//Print << U"canDrop = {}"_fmt(canDrop);
	// 操作可能
	if (canOperate)
	{
		// オジャマを落とす
		if (canDrop && obstructsSentSum > 0)
		{
			fieldMoveTo = field.getFloatTo(obstructsSentCntArray);

			for (auto p : step(fieldSize))
			{
				obstructsCntDrawField.at(p) = p.y >= fieldSize.y - obstructsSentCntArray[p.x]
					? Cell(CellType::Obstruct) : Cell(CellType::Empty);
			}

			obstructsSentSum = 0;
			for (auto x : step(fieldSize.x))
			{
				obstructsSentCntArray.at(x) = 0;
			}

			// セル押上処理へ移行
			canDrop = false;
			isPushedTime = true;
		}

		// フィールドをランダムに変更する
		if (canDrop && keySet.toRandom.down())
		{
			field = CellField::RandomField(fieldSize, (int32)dropCells1LoopNum.size() - 1, false, false);
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
			if (holdCell.getType() == CellType::Empty)
			{
				holdCell = getDropCell(0);
				dropCellStack.remove_at(0);
			}
			else
			{
				Cell tmp = holdCell;
				holdCell = dropCellStack.at(0);
				dropCellStack.at(0) = tmp;
			}
		}

		// 落とすセルを変更する
		if (keySet.changeCell.down())
		{
			// 1 ~ cellMaxNumberで1周させる
			int32 nextNumber = static_cast<int32>(getDropCell(0).getType()) + 1;
			if (nextNumber >= dropCells1LoopNum.size())
				nextNumber = 1;
			dropCellStack.at(0) = Cell(static_cast<CellType>(nextNumber));
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
			field.pushTopCell(getDropCell(0), dropCellFieldX);

			// 落下処理へ移行
			canDrop = false;
			isFallingTime = true;
			fieldMoveTo = field.getFallTo();

			dropCellStack.remove_at(0);
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
				// 有効Just10Timesの合計
				int32 dc = 0;
				for (auto p : step(fieldSize))
				{
					if (field.getField().at(p).getType() != CellType::Obstruct)
					{
						dc += deleteField.at(p);
					}
				}

				field.deleteCells(deleteField);
				if (Setting::debug_print)	Print << U"Delete {} Cells!"_fmt(dc);

				// コンボ倍率においては要調整
				int32 scoreAdd = scoreFunc(dc, combo);
				score += scoreAdd;
				obstructsMaked += scoreAdd;
				combo++;

				isDeletingTime = false;
				deletingTimer = 0.0;

				// 落下処理へ移行
				isFallingTime = true;
				fieldMoveTo = field.getFallTo();
				if (Setting::debug_print)	Print << U"Falling...";
			}
		}

		// セルが落ちる演出
		if (isFallingTime)
		{
			fallingTimer += deltaTime;

			if (fallingTimer > fallingCoolTime)
			{
				field.moveCells(fieldMoveTo);
				if (Setting::debug_print)	Print << U"Falled Cells!";

				isFallingTime = false;
				fallingTimer = 0.0;

				// 消えるものがないなら、動かせるようにする
				if (!updatedField())
				{
					canDrop = true;
					combo = 0;
					ret = obstructsMaked;
					obstructsMaked = 0;
				}
			}
		}

		// セルがオジャマに押され上がる演出
		if (isPushedTime)
		{
			pushedTimer += deltaTime;

			if (pushedTimer > pushedCoolTime)
			{
				field.moveCells(fieldMoveTo);
				for (auto p : step(fieldSize))
				{
					if (obstructsCntDrawField.at(p).getType() != CellType::Empty)
					{
						field.setCell(obstructsCntDrawField.at(p), p);
						obstructsCntDrawField.at(p) = Cell(CellType::Empty);
					}
				}
				if (Setting::debug_print)	Print << U"for (auto p : step(fieldSize))";

				isPushedTime = false;
				pushedTimer = 0.0;

				// 消えるものがないなら、動かせるようにする
				if (!updatedField())
				{
					canDrop = true;
				}
			}
		}

		// 負けと表示する演出
		if (state == BattleState::lose)
		{
			loseTimer += deltaTime;
		}
	}

	return ret;
}

void Player::draw(Point fieldPos, Size cellDrawSize) const
{
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
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).draw(fieldColor);
	// フィールドの枠
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).drawFrame(10, ColorF(0.2));

	// Just10の回数の表示
	if (KeyControl.pressed())
	{
		for (auto p : step(fieldSize))
		{
			Point pos = fieldPos + cellDrawSize * p + cellDrawSize / 2;
			FontAsset(U"Text")(U"{}"_fmt(deleteField.at(p))).drawAt(pos, Palette::Black);
		}
	}
	// フィールドの描画
	else
	{
		for (int32 x : step(fieldSize.x))
		{
			Point pos = fieldPos + Point(x, 0) * cellDrawSize;

			Cell::getTexture(CellType::No).resized(cellDrawSize).draw(pos);
		}

		for (auto p : step(fieldSize))
		{
			if (obstructsCntDrawField.at(p).getType() != CellType::Empty)
			{
				Point pos = fieldPos + p * cellDrawSize;
				Cell::getTexture(obstructsCntDrawField.at(p).getType()).resized(cellDrawSize).draw(pos);
			}
		}

		field.draw(fieldPos, cellDrawSize,
			[&](Point p, CellType) {
				if (fieldMoveTo.at(p) == Point(-1, -1))
				{
					return (p * cellDrawSize);
				}
				else
				{
					const double e = EaseOutCubic(
						isFallingTime ? fallingTimer / fallingCoolTime :
						isDeletingTime ? deletingTimer / deletingCoolTime :
						0);
					return (Vec2(p * cellDrawSize).lerp(Vec2(fieldMoveTo.at(p) * cellDrawSize), e)).asPoint();
				}
			},
			[&](Point p, CellType) {
				if (deleteField.at(p))
				{
					const double e = EaseOutCubic(deletingTimer / deletingCoolTime);
					return Color(255, (int32)(255 * (1.0 - e)));
				}
				else
				{
					return Color(255);
				}
			}
			);
	}

	// 落とすセルの描画
	{
		const Point dropCellPos(fieldPos + Point(cellDrawSize.x * dropCellFieldX, -cellDrawSize.y));

		getDropCellConst(0).getTexture().resized(cellDrawSize).draw(dropCellPos);
	}
}

void Player::makeDropCells(int32 min)
{
	// dropCellsを追加
	while (min >= dropCellStack.size())
	{
		dropCellStack.append(dropCells1LoopCells.shuffled());
	}
}

bool Player::updatedField()
{
	deleteField = field.getJust10Times();

	// 消えるセルの周りにあるオジャマも消す
	for (auto p : step(fieldSize))
	{
		if (field.getField().at(p).getType() == CellType::Obstruct)
		{
			for (auto i : step(1, 4, 2))
			{
				Point padd = Point(i / 3 - 1, i % 3 - 1);
				if (0 <= p.x + padd.x && p.x + padd.x < fieldSize.x
					&& 0 <= p.y + padd.y && p.y + padd.y < fieldSize.y
					&& field.getField().at(p + padd).getType() != CellType::Obstruct
					&& deleteField.at(p + padd) > 0)
				{
					deleteField.at(p) = 1;
				}
			}
		}
	}


	// Just10で消えるものがあるなら、消える時間にする
	if (deleteField.any([](int32 n) { return n != 0; }))
	{
		canDrop = false;
		isDeletingTime = true;
		if (Setting::debug_print)	Print << U"Deleting...";

		return true;
	}

	// 一番上のyにセルがあるなら、負け
	for (auto x : step(field.getSize().x))
	{
		if (field.getField().at(0, x).getType() != CellType::Empty)
		{
			canOperate = false;
			state = BattleState::lose;
			return false;
		}
	}

	return false;
}

void Player::sendObstructs(double sent_obstructs)
{
	int32 obstructs = static_cast<int32>(ceil(sent_obstructs / defRate));

	obstructsSentSum += obstructs;
	for (auto i : step(fieldSize.x))
	{
		obstructsSentCntArray.at(i) += obstructs / fieldSize.x;
	}

	obstructs %= fieldSize.x;

	Array<int32> put(fieldSize.x, 0);
	for (auto i : step(fieldSize.x))
	{
		put.at(i) = i;
	}

	for (auto i : put.choice(obstructs))
	{
		if (Setting::debug_print)	Print << U"i = {}"_fmt(i);
		++obstructsSentCntArray.at(i);
	}
}
