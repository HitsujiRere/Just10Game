
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
	if (Setting::debugPrint)	Print << U"called update()";

	int32 ret = 0;

	const double deltaTime = Scene::DeltaTime();

	// ���10�ȏ�m�ۂ��Ă����iNext�\���΍�j
	getDropCell(10);

	//Print << U"canDrop = {}"_fmt(canDrop);
	// ����\
	if (canOperate)
	{
		if (Setting::debugPrint)	Print << U"if (canOperate)";
		try
		{
			// �I�W���}�𗎂Ƃ�
			if (canDrop && obstructsSentSum > 0)
			{
				fieldMoveTo = field.getFloatTo(obstructsSentCntArray);
				if (Setting::debugPrint)
				{
					Print << U"fieldSize.y - obstructsSentCntArray[x] = ";
					for (auto x : step(fieldSize.x))
					{
						Print(Format(fieldSize.y - obstructsSentCntArray[x]) + U", ");
					}
					Print << U"";
				}
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

				// �Z�����㏈���ֈڍs
				canDrop = false;
				isPushedTime = true;
				if (Setting::debugPrint)	Print << U"Pushed up...";

				if (Setting::debugPrint)	Print << U"fieldMoveTo";
				if (Setting::debugPrint)	Print << fieldMoveTo;
				if (Setting::debugPrint)	Print << U"obstructsSentCntArray";
				if (Setting::debugPrint)	Print << obstructsSentCntArray;
				if (Setting::debugPrint)	Print << U"obstructsCntDrawField";
				if (Setting::debugPrint)	Print << obstructsCntDrawField;
			}
		}
		catch (std::exception & e)
		{
			Print << Unicode::Widen(e.what()) << U" in if (canOperate)";
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
			if (holdCell.getNumber() == (int32)CellType::Empty)
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

			if (Setting::debugPrint)	Print << U"Hold...";
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
			if (Setting::debugPrint)	Print << U"Falling...";

			dropCells.remove_at(0);
		}
	}

	// ���o����
	{
		if (Setting::debugPrint)	Print << U"in ���o����{}";

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
					if (field.getGrid().at(p).getNumber() != (int32)CellType::Obstruct)
					{
						dc += deleteField.at(p);
					}
				}

				field.deleteCells(deleteField);
				if (Setting::debugPrint)	Print << U"Delete {} Cells!"_fmt(dc);

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
				if (Setting::debugPrint)	Print << U"Falling...";
			}
		}

		// �Z���������鉉�o
		if (isFallingTime)
		{
			fallingTimer += deltaTime;

			if (fallingTimer > fallingCoolTime)
			{
				field.moveCells(fieldMoveTo);
				if (Setting::debugPrint)	Print << U"Falled Cells!";

				isFallingTime = false;
				fallingTimer = 0.0;

				// ��������̂��Ȃ��Ȃ�A��������悤�ɂ���
				if (!updatedField())
				{
					canDrop = true;
					combo = 0;
					ret = obstructsMaked;
					obstructsMaked = 0;
				}
			}
		}

		// �Z�����I�W���}�ɉ�����オ�鉉�o
		try
		{
			//Print << U"try";
			if (Setting::debugPrint)	Print << U"isPushedTime = {}"_fmt(isPushedTime);
			if (Setting::debugPrint)	Print << U"canDrop = {}"_fmt(canDrop);
			if (isPushedTime)
			{
				pushedTimer += deltaTime;

				if (pushedTimer > pushedCoolTime)
				{
					if (Setting::debugPrint || true)	Print << U"fieldSize = {}"_fmt(fieldSize);
					if (Setting::debugPrint || true)	Print << U"fieldMoveTo size = {}"_fmt(fieldMoveTo.size());
					if (Setting::debugPrint || true)	Print << U"field.moveCells()";
					field.moveCells(fieldMoveTo);
					if (Setting::debugPrint)	Print << U"field.moveCells(fieldMoveTo);";
					if (Setting::debugPrint)	Print << U"obstructsCntDrawField size = {}"_fmt(obstructsCntDrawField.size());
					for (auto p : step(fieldSize))
					{
						if (obstructsCntDrawField.at(p).getNumber() != (int32)CellType::Empty)
						{
							field.setCell(obstructsCntDrawField.at(p), p);
							obstructsCntDrawField.at(p) = Cell(CellType::Empty);
						}
					}
					if (Setting::debugPrint)	Print << U"for (auto p : step(fieldSize))";

					isPushedTime = false;
					pushedTimer = 0.0;

					// ��������̂��Ȃ��Ȃ�A��������悤�ɂ���
					if (!updatedField())
					{
						canDrop = true;
					}
				}
			}
			//Print << U"isPushedTime = {}"_fmt(isPushedTime);
			//Print << U"canDrop = {}"_fmt(canDrop);
		}
		catch (std::exception & e)
		{
			Print << Unicode::Widen(e.what()) << U" in if (isPushedTime)";
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

			Cell::getTexture((int32)CellType::No).resized(cellDrawSize).draw(pos);
		}

		try
		{
			for (auto p : step(fieldSize))
			{
				if (obstructsCntDrawField.at(p).getNumber() != (int32)CellType::Empty)
				{
					Point pos = fieldPos + p * cellDrawSize;
					Cell::getTexture(obstructsCntDrawField.at(p).getNumber()).resized(cellDrawSize).draw(pos);
				}
			}
		}
		catch (std::exception & e)
		{
			Print << Unicode::Widen(e.what()) << U" in draw()";
		}

		field.draw(fieldPos, cellDrawSize,
			[&](Point p, int32) {
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
			[&](Point p, int32) {
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
		if (field.getGrid().at(p).getNumber() == (int32)CellType::Obstruct)
		{
			for (auto i : step(1, 4, 2))
			{
				Point padd = Point(i / 3 - 1, i % 3 - 1);
				if (0 <= p.x + padd.x && p.x + padd.x < fieldSize.x
					&& 0 <= p.y + padd.y && p.y + padd.y < fieldSize.y
					&& field.getGrid().at(p + padd).getNumber() != (int32)CellType::Obstruct
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
		if (Setting::debugPrint)	Print << U"Deleting...";

		return true;
	}

	// ��ԏ��y�ɃZ��������Ȃ�A����
	for (auto x : step(field.size().x))
	{
		if (field.getGrid().at(0, x).getNumber() != (int32)CellType::Empty)
		{
			canOperate = false;
			state = BattleState::lose;
			return false;
		}
	}

	return false;
}

void Player::sendObstructs(int32 obstructs, double atkRate)
{
	obstructs = (int32)ceil(obstructs * atkRate / defRate);

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
	if (Setting::debugPrint)	Print << U"put";
	if (Setting::debugPrint)	Print << put;
	for (auto i : put.choice(obstructs))
	{
		if (Setting::debugPrint)	Print << U"i = {}"_fmt(i);
		++obstructsSentCntArray.at(i);
	}
}
