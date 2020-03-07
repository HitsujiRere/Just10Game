
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

	// ���10�ȏ�m�ۂ��Ă����iNext�\���΍�j
	getDropCell(10);

	// ����\
	if (canOperate)
	{
		// �t�B�[���h�������_���ɕύX����
		if (canDrop && keySet.toRandom.down())
		{
			field = CellField::RandomField(fieldSize, dropCells1LoopNum.size() - 1, false, false);
			updatedField();
		}

		// �t�B�[���h�𖳂ɕύX����
		if (canDrop && keySet.toEmpty.down())
		{
			field = CellField(fieldSize);
			updatedField();
		}

		// �Z�����z�[���h����
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

		// ���Ƃ��Z����ύX����
		if (keySet.changeCell.down())
		{
			// 1 ~ cellMaxNumber��1������
			int32 nextNumber = getDropCell(0).getNumber() + 1;
			if (nextNumber >= dropCells1LoopNum.size())
				nextNumber = 1;
			getDropCell(0) = Cell(nextNumber);
		}

		// ���Ƃ��Z�����ړ�����
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

		//�Z���𗎉�������
		if (canDrop && keySet.drop.pressed())
		{
			field.pushCell(getDropCell(0), dropCellFieldX);

			// ���������ֈڍs
			canDrop = false;
			isFallingTime = true;
			fieldMoveTo = field.getFallTo();
			if (debugPrint)	Print << U"Falling...";

			dropCells.remove_at(0);
		}
	}

	// ���o����
	{
		// �Z���������鉉�o
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

				// ���������ֈڍs
				isFallingTime = true;
				fieldMoveTo = field.getFallTo();
				if (debugPrint)	Print << U"Falling...";
			}
		}

		// �Z���������鉉�o
		if (isFallingTime)
		{
			fallingTimer += deltaTime;

			if (fallingTimer > fallingCoolTime)
			{
				field.moveCells(fieldMoveTo);
				if (debugPrint)	Print << U"Falled Cells!";

				isFallingTime = false;
				fallingTimer = 0.0;

				// ��������̂�����Ȃ�A�����鉉�o��
				if (!updatedField())
				{
					canDrop = true;
				}
			}
		}

		// �����ƕ\�����鉉�o
		if (state == (int32)BattleState::lose)
		{
			loseTimer += deltaTime;
		}
	}
}

void Player::draw(Point fieldPos, Size cellDrawSize) const
{
	// �X�R�A�̕\��
	FontAsset(U"Text")(U"Score:{}"_fmt(m_score))
		.drawAt(fieldPos.movedBy(fieldSize.x * cellDrawSize.x / 2, fieldSize.y * cellDrawSize.y + 30), ColorF(0.25));

	// Next�Z���̕`��
	FontAsset(U"Text")(U"Next").drawAt(fieldPos + Vec2(fieldSize.x + 2, -0.5) * cellDrawSize);
	getDropCellConst(1).getTexture().resized(cellDrawSize * 2).draw(fieldPos + Vec2(fieldSize.x + 1, 0) * cellDrawSize);
	for (auto i : step(2, 4))
	{
		getDropCellConst(i).getTexture().resized(cellDrawSize).draw(fieldPos + Vec2(fieldSize.x + 1, i) * cellDrawSize);
	}

	// �z�[���h�̕\��
	FontAsset(U"Text")(U"Hold").drawAt(fieldPos + Vec2(-2, -0.5) * cellDrawSize);
	holdCell.getTexture().resized(cellDrawSize * 2).draw(fieldPos + Point(-3, 0) * cellDrawSize);

	// �t�B�[���h�̔w�i
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).draw(ColorF(0.2, 0.8, 0.4));
	// �t�B�[���h�̘g
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).drawFrame(10, Palette::Forestgreen);

	// Just10�̉񐔂̕\��
	if (KeyControl.pressed())
	{
		for (auto p : step(fieldSize))
		{
			Point pos = fieldPos + cellDrawSize * p + cellDrawSize / 2;
			FontAsset(U"Text")(U"{}"_fmt(just10Times.at(p))).draw(pos, Palette::Black);
		}
	}
	// �t�B�[���h�̕`��
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

	// ���Ƃ��Z���̕`��
	{
		const Point dropCellPos(fieldPos + Point(cellDrawSize.x * dropCellFieldX, -cellDrawSize.y));

		getDropCellConst(0).getTexture().resized(cellDrawSize).draw(dropCellPos);
	}

	// �����Ƃ����m�点�̕`��
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
	// dropCells��ǉ�
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

	// Just10�ŏ�������̂�����Ȃ�A�����鎞�Ԃɂ���
	if (just10Times.any([](int32 n) { return n != 0; }))
	{
		canDrop = false;
		isDeletingTime = true;
		if (debugPrint)	Print << U"Deleting...";

		return true;
	}

	// ��ԏ��y�ɃZ��������Ȃ�A����
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
