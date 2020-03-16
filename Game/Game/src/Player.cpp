
#include "Player.hpp"

Player::Player(Array<int32> _dropCells1LoopNum)
	: dropCells1LoopNum(_dropCells1LoopNum)
	, field(Point(6, 12))
	, just10times(field.getSize())
	, fieldMoveTo(field.getSize())
	, obstructsSentCntArray(field.getSize().x)
{
	for (int32 i : step(static_cast<int32>(dropCells1LoopNum.size())))
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
	this->just10times = another.just10times;
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
	this->isMovingTime = another.isMovingTime;
	this->movingTimer = another.movingTimer;
	this->loseTimer = another.loseTimer;
	this->score = another.score;
	this->combo = another.combo;
	this->state = another.state;

	return *this;
}

int32 Player::update(PlayerKeySet keySet)
{
	if (Setting::debugPrint)	Print << U"\t\tPlayer::update() begin";
	int32 ret = 0;

	const Size size = field.getSize();
	const Size drawsize = field.getDrawsize();

	const double deltaTime = Scene::DeltaTime();

	// ���10�ȏ�m�ۂ��Ă����iNext�\���΍�j
	makeDropCells(10);

	if (KeyTab.down())
	{
		updatedField();
	}

	if (Setting::debugPrint)	Print << U"\t\t\t- 1";

	//Print << U"canDrop = {}"_fmt(canDrop);
	// ����\
	if (canOperate)
	{
		// �I�W���}������
		if (canDrop && obstructsSentSum > 0)
		{
			for (auto x : step(size.x))
			{
				for (auto y : step(drawsize.y, obstructsSentCntArray.at(x)))
				{
					Cell cell(CellType::Obstruct);
					field.setCell(cell, x, y);
				}
			}

			fieldMoveTo = field.getFloatTo(obstructsSentCntArray);

			obstructsSentSum = 0;
			for (auto x : step(drawsize.x))
			{
				obstructsSentCntArray.at(x) = 0;
			}

			// �Z���ړ������ֈڍs
			canDrop = false;
			isMovingTime = true;
			//fieldMoveTo = field.getFallTo();
		}

		// �t�B�[���h�������_���ɕύX����
		if (canDrop && keySet.toRandom.down())
		{
			field = CellField::RandomField(drawsize, static_cast<int32>(dropCells1LoopNum.size()) - 1, false, false);
			updatedField();
		}

		// �t�B�[���h�𖳂ɕύX����
		if (canDrop && keySet.toEmpty.down())
		{
			field = CellField(drawsize);
			updatedField();
		}

		// �Z�����z�[���h����
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

		// ���Ƃ��Z����ύX����
		if (keySet.changeCell.down())
		{
			// 1 ~ cellMaxNumber��1��������
			int32 nextNumber = static_cast<int32>(getDropCell(0).getType()) + 1;
			if (nextNumber >= dropCells1LoopNum.size())
				nextNumber = 1;
			dropCellStack.at(0) = Cell(static_cast<CellType>(nextNumber));
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
		if (keySet.moveR.pressed() && dropCellFieldX < drawsize.x - 1)
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
			isMovingTime = true;
			fieldMoveTo = field.getFallTo();

			dropCellStack.remove_at(0);
		}
	}

	if (Setting::debugPrint)	Print << U"\t\t\t- 2";

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
				for (auto p : step(drawsize))
				{
					if (field.getField().at(p).getType() != CellType::Obstruct)
					{
						dc += just10times.at(p);
					}
				}

				if (Setting::debugPrint)	Print << U"\t\t\t- 2 - 0 - 1";

				field.deleteCells(just10times);
				if (Setting::debugPrint)	Print << U"Delete {} Cells!"_fmt(dc);

				if (Setting::debugPrint)	Print << U"\t\t\t- 2 - 0 - 2";

				// �R���{�{���ɂ����Ă͗v����
				int32 scoreAdd = scoreFunc(dc, combo);
				score += scoreAdd;
				obstructsMaked += scoreAdd;
				combo++;

				isDeletingTime = false;
				deletingTimer = 0.0;

				if (Setting::debugPrint)	Print << U"\t\t\t- 2 - 0 - 3";

				// ���������ֈڍs
				isMovingTime = true;
				fieldMoveTo = field.getFallTo();
				if (Setting::debugPrint)	Print << U"Falling...";
			}
		}

		if (Setting::debugPrint)	Print << U"\t\t\t- 2 - 1";

		// �Z�����������o
		if (isMovingTime)
		{
			movingTimer += deltaTime;

			if (movingTimer > movingCoolTime)
			{
				if (Setting::debugPrint)	Print << U"\t\t\t- 2 - 1 - 1";

				field.moveCells(fieldMoveTo);
				if (Setting::debugPrint)	Print << U"Falled Cells!";

				isMovingTime = false;
				movingTimer = 0.0;

				if (Setting::debugPrint)	Print << U"\t\t\t- 2 - 1 - 2";

				// ��������̂��Ȃ��Ȃ�A��������悤�ɂ���
				if (!updatedField())
				{
					canDrop = true;
					combo = 0;
					ret = obstructsMaked;
					obstructsMaked = 0;
				}

				if (Setting::debugPrint)	Print << U"\t\t\t- 2 - 1 - 3";
			}

			// �����ƕ\�����鉉�o
			if (state == BattleState::lose)
			{
				loseTimer += deltaTime;
			}
		}

		if (Setting::debugPrint)	Print << U"\t\t\t- 2 - 2";
	}

	if (Setting::debugPrint)	Print << U"\t\tPlayer::update() end";
	return ret;
}

void Player::draw(Point fieldPos, Size cellSize) const
{
	if (Setting::debugPrint)	Print << U"\t\tdraw() begin";

	const Size drawsize = field.getDrawsize();

	// Next�Z���̕`��
	FontAsset(U"Text")(U"Next").drawAt(fieldPos + Vec2(drawsize.x + 2, -0.5) * cellSize);
	getDropCellConst(1).getTexture().resized(cellSize * 2).draw(fieldPos + Vec2(drawsize.x + 1, 0) * cellSize);
	for (auto i : step(2, 4))
	{
		getDropCellConst(i).getTexture().resized(cellSize).draw(fieldPos + Vec2(drawsize.x + 1, i) * cellSize);
	}

	if (Setting::debugPrint)	Print << U"\t\t\t- 1";

	// �z�[���h�̕\��
	FontAsset(U"Text")(U"Hold").drawAt(fieldPos + Vec2(-2, -0.5) * cellSize);
	holdCell.getTexture().resized(cellSize * 2).draw(fieldPos + Point(-3, 0) * cellSize);

	if (Setting::debugPrint)	Print << U"\t\t\t- 2";

	// �t�B�[���h�̔w�i
	Rect(fieldPos - cellSize * Size(0, 1) - Point(5, 5), (drawsize + Size(0, 1)) * cellSize + Point(10, 10)).draw(fieldColor);
	// �t�B�[���h�̘g
	Rect(fieldPos - cellSize * Size(0, 1) - Point(5, 5), (drawsize + Size(0, 1)) * cellSize + Point(10, 10)).drawFrame(10, ColorF(0.2));

	if (Setting::debugPrint)	Print << U"\t\t\t- 3";

	// Just10�̉񐔂̕\��
	if (KeyControl.pressed())
	{
		for (auto p : step(drawsize))
		{
			Point pos = fieldPos + cellSize * p + cellSize / 2;
			FontAsset(U"Text")(U"{}"_fmt(just10times.at(p))).drawAt(pos, Palette::Black);
		}
	}
	// �t�B�[���h�̕`��
	else
	{
		for (int32 x : step(drawsize.x))
		{
			Point pos = fieldPos + Point(x, 0) * cellSize;

			Cell::getTexture(CellType::No).resized(cellSize).draw(pos);
		}

		if (Setting::debugPrint)	Print << U"\t\t\t- 3 - 1";

		/**
		if (isPushedTime)
		{
			for (auto p : step(drawsize))
			{
				if (obstructsCntDrawField.at(p).getType() != CellType::Empty)
				{
					Point pos = fieldPos + p * cellSize;
					Cell::getTexture(obstructsCntDrawField.at(p).getType()).resized(cellSize).draw(pos);
				}
			}
		}
		/**/

		if (Setting::debugPrint)	Print << U"\t\t\t- 3 - 2";

		field.draw(fieldPos, cellSize,
			[&](Point p, CellType) {
				if (fieldMoveTo.at(p) == Point(-1, -1))
				{
					return (p * cellSize);
				}
				else
				{
					const double e = EaseOutCubic(
						isMovingTime ? movingTimer / movingCoolTime :
						isDeletingTime ? deletingTimer / deletingCoolTime :
						0);
					return (Vec2(p * cellSize).lerp(Vec2(fieldMoveTo.at(p) * cellSize), e)).asPoint();
				}
			},
			[&](Point p, CellType) {
				if (just10times.at(p))
				{
					const double e = EaseOutCubic(deletingTimer / deletingCoolTime);
					return Color(255, static_cast<int32>(255 * (1.0 - e)));
				}
				else
				{
					return Color(255);
				}
			}
			);
	}

	if (Setting::debugPrint)	Print << U"\t\t\t- 4";

	// ���Ƃ��Z���̕`��
	{
		const Point dropCellPos(fieldPos + Point(cellSize.x * dropCellFieldX, -cellSize.y));

		getDropCellConst(0).getTexture().resized(cellSize).draw(dropCellPos);
	}

	if (Setting::debugPrint)	Print << U"\t\tdraw() end";
}

void Player::makeDropCells(int32 min)
{
	// dropCells��ǉ�
	while (min >= dropCellStack.size())
	{
		dropCellStack.append(dropCells1LoopCells.shuffled());
	}
}

bool Player::updatedField()
{
	const Size drawsize = field.getDrawsize();

	just10times = field.getJust10Times();

	// ������Z���̎���ɂ���I�W���}������
	for (auto p : step(drawsize))
	{
		if (field.getField().at(p).getType() == CellType::Obstruct)
		{
			for (auto i : step(1, 4, 2))
			{
				Point padd = Point(i / 3 - 1, i % 3 - 1);
				if (0 <= p.x + padd.x && p.x + padd.x < drawsize.x
					&& 0 <= p.y + padd.y && p.y + padd.y < drawsize.y
					&& field.getField().at(p + padd).getType() != CellType::Obstruct
					&& just10times.at(p + padd) > 0)
				{
					just10times.at(p) = 1;
				}
			}
		}
	}

	// Just10�ŏ�������̂�����Ȃ�A�����鎞�Ԃɂ���
	if (just10times.any([](int32 n) { return n != 0; }))
	{
		canDrop = false;
		isDeletingTime = true;
		if (Setting::debugPrint)	Print << U"Deleting...";

		return true;
	}

	// ��ԏ��y�ɃZ��������Ȃ�A����
	for (auto x : step(drawsize.x))
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
	if (Setting::debugPrint)	Print << U"\t\tsendObstructs() begin";

	const Size size = field.getDrawsize();

	int32 obstructs = static_cast<int32>(ceil(sent_obstructs / defRate));

	if (Setting::debugPrint)	Print << U"\t\t- 1";

	obstructsSentSum += obstructs;
	for (auto i : step(size.x))
	{
		obstructsSentCntArray.at(i) += obstructs / size.x;
	}

	if (Setting::debugPrint)	Print << U"\t\t- 2";

	obstructs %= size.x;

	Array<int32> put(size.x, 0);
	for (auto i : step(size.x))
	{
		put.at(i) = i;
	}

	if (Setting::debugPrint)	Print << U"\t\t- 3";

	for (auto i : put.choice(obstructs))
	{
		++obstructsSentCntArray.at(i);
	}

	if (Setting::debugPrint)	Print << U"\t\tsendObstructs() end";
}
