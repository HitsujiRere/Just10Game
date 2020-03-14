
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

Player& Player::operator=(const Player& another)
{
	this->dropCells1LoopNum = another.dropCells1LoopNum;
	this->dropCells1LoopCells = another.dropCells1LoopCells;
	this->debugPrint = another.debugPrint;
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

	// ���10�ȏ�m�ۂ��Ă����iNext�\���΍�j
	getDropCell(10);

	// ����\
	if (canOperate)
	{
		// �I�W���}�𗎂Ƃ�
		if (obstructsSentSum > 0)
		{
			for (auto x : step(fieldSize.x))
			{
				if (obstructsSentArray.at(x) > 0)
				{
					field.pushUnderObsructs(x, (int32)(obstructsSentArray.at(x) * sendObstrucePer));
					obstructsSentSum -= obstructsSentArray.at(x);
					obstructsSentArray.at(x) = 0;
				}
			}
		}

		// �t�B�[���h�������_���ɕύX����
		if (canDrop && keySet.toRandom.down())
		{
			field = CellField::RandomField(fieldSize, (int32)dropCells1LoopNum.size() - 1, false, false);
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
			field.pushTopCell(getDropCell(0), dropCellFieldX);

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
				// �L��Just10Times�̍��v
				int32 dc = 0;
				for (auto p : step(fieldSize))
				{
					if (field.getGrid().at(p).getNumber() != (int32)CellTypeNumber::Obstruct)
					{
						dc += deleteField.at(p);
					}
				}

				field.deleteCells(deleteField);
				if (debugPrint)	Print << U"Delete {} Cells!"_fmt(dc);

				// �R���{�{���ɂ����Ă͗v����
				int32 scoreAdd = scoreFunc(dc, combo);
				score += scoreAdd;
				obstructsMaked += scoreAdd;
				combo++;

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

				// ��������̂�����Ȃ��Ȃ�A��������悤�ɂ���
				if (!updatedField())
				{
					canDrop = true;
					combo = 0;
					ret = obstructsMaked;
					obstructsMaked = 0;
				}
			}
		}

		// �����ƕ\�����鉉�o
		if (state == BattleState::lose)
		{
			loseTimer += deltaTime;
		}
	}

	return ret;
}

void Player::draw(Point fieldPos, Size cellDrawSize) const
{
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
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).draw(fieldColor);
	// �t�B�[���h�̘g
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).drawFrame(10, ColorF(0.2));

	// Just10�̉񐔂̕\��
	if (KeyControl.pressed())
	{
		for (auto p : step(fieldSize))
		{
			Point pos = fieldPos + cellDrawSize * p + cellDrawSize / 2;
			FontAsset(U"Text")(U"{}"_fmt(deleteField.at(p))).drawAt(pos, Palette::Black);
		}
	}
	// �t�B�[���h�̕`��
	else
	{
		for (int32 x : step(fieldSize.x))
		{
			Point pos = fieldPos + Point(x, 0) * cellDrawSize;

			Cell::getTexture((int32)CellTypeNumber::No).resized(cellDrawSize).draw(pos);
		}

		field.draw(fieldPos, cellDrawSize,
			[&](Point p, int32) {
				if (deleteField.at(p))
					return (p * cellDrawSize);
				else
				{
					const double e = EaseOutCubic(fallingTimer / fallingCoolTime);
					return (Vec2(p * cellDrawSize).lerp(Vec2(fieldMoveTo.at(p) * cellDrawSize), e)).asPoint();
				}
			},
			[&](Point p, int32) {
				if (deleteField.at(p))
				{
					const double e = EaseOutCubic(deletingTimer / deletingCoolTime);
					return Color(255, (int32)(255 * (1.0 - e)));
				}
				else
					return Color(255);
			}
			);
	}

	// ���Ƃ��Z���̕`��
	{
		const Point dropCellPos(fieldPos + Point(cellDrawSize.x * dropCellFieldX, -cellDrawSize.y));

		getDropCellConst(0).getTexture().resized(cellDrawSize).draw(dropCellPos);
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
	deleteField = field.getJust10Times();

	// ������Z���̎���ɂ���I�W���}������
	for (auto p : step(fieldSize))
	{
		if (field.getGrid().at(p).getNumber() == (int32)CellTypeNumber::Obstruct)
		{
			for (auto i : step(1, 4, 2))
			{
				Point padd = Point(i / 3 - 1, i % 3 - 1);
				if (0 <= p.x + padd.x && p.x + padd.x < fieldSize.x
					&& 0 <= p.y + padd.y && p.y + padd.y < fieldSize.y
					&& field.getGrid().at(p + padd).getNumber() != (int32)CellTypeNumber::Obstruct
					&& deleteField.at(p + padd) > 0)
				{
					deleteField.at(p) = 1;
				}
			}
		}
	}


	// Just10�ŏ�������̂�����Ȃ�A�����鎞�Ԃɂ���
	if (deleteField.any([](int32 n) { return n != 0; }))
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
			state = BattleState::lose;
			return false;
		}
	}

	return false;
}

void Player::sendObstructs(int32 obstructs)
{
	obstructs = (int32)(obstructs * sentObstrucePer);

	obstructsSentSum += obstructs;
	for (auto i : step(fieldSize.x))
	{
		obstructsSentArray.at(i) += obstructs / fieldSize.x;
	}
	obstructs %= fieldSize.x;
	Array<int32> put(fieldSize.x);
	for (auto i : step(fieldSize.x))
	{
		put.at(i) = i;
	}
	for (auto i : put.choice(obstructs))
	{
		++obstructsSentArray.at(i);
	}
}
