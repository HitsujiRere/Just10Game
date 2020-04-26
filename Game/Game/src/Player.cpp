
#include "Player.hpp"

Player::Player(Array<int32> dropCells1LoopNum)
	: field(Point(6, 12))
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
	this->dropCells1LoopCells = another.dropCells1LoopCells;
	this->dropCellStack = another.dropCellStack;

	this->field = another.field;
	this->just10times = another.just10times;
	this->fieldMoveTo = another.fieldMoveTo;

	this->fieldColor = another.fieldColor;

	this->dropCellFieldX = another.dropCellFieldX;

	this->holdCell = another.holdCell;

	this->canOperate = another.canOperate;
	this->canDrop = another.canDrop;

	this->isDeletingTime = another.isDeletingTime;
	this->deletingTimer = another.deletingTimer;

	this->isMovingTime = another.isMovingTime;
	this->movingTimer = another.movingTimer;

	this->stateTimer = another.stateTimer;

	this->state = another.state;

	this->score = another.score;
	this->scoreFunc = another.scoreFunc;
	this->combo = another.combo;

	this->obstructsMakedCnt = another.obstructsMakedCnt;
	this->obstructsSentSum = another.obstructsSentSum;
	this->obstructsSentCntArray = another.obstructsSentCntArray;

	this->sendingObstructCnt = another.sendingObstructCnt;
	this->sendingObstructTimer = another.sendingObstructTimer;

	return *this;
}

void Player::update(std::shared_ptr<PlayerOperator> operaterPtr)
{
	const Size size = field.getSize();
	const Size drawsize = field.getDrawsize();

	const double deltaTime = Scene::DeltaTime();

	// ���10�ȏ�m�ۂ��Ă����iNext�\���΍�j
	makeDropCells(10);

	if (KeyTab.down())
	{
		updatedField();
	}

	// ����\
	if (canOperate)
	{
		// �I�W���}�𐶐�����
		if (canDrop)
		{
			makeObstruct();
		}

		// �Z�����z�[���h����
		if (operaterPtr->isHold())
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

		// ���Ƃ��Z�����ړ�����
		if (operaterPtr->isMoveL() && dropCellFieldX > 0)
		{
			dropCellFieldX--;
		}
		if (operaterPtr->isMoveR() && dropCellFieldX < drawsize.x - 1)
		{
			dropCellFieldX++;
		}

		//�Z���𗎉�������
		if (canDrop && operaterPtr->isDrop())
		{
			field.pushTopCell(getDropCell(0), dropCellFieldX);

			// ���������ֈڍs
			canDrop = false;
			isMovingTime = true;
			fieldMoveTo = field.getFallTo();

			dropCellStack.remove_at(0);
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
				for (auto p : step(drawsize))
				{
					if (field.getField().at(p).getType() != CellType::Obstruct)
					{
						dc += just10times.at(p);
					}
				}

				field.deleteCells(just10times);

				// �R���{�{���ɂ����Ă͗v����
				int32 scoreAdd = scoreFunc(dc, combo);
				score += scoreAdd;
				obstructsMakedCnt += scoreAdd;
				combo++;

				isDeletingTime = false;
				deletingTimer = 0.0;

				// ���������ֈڍs
				isMovingTime = true;
				fieldMoveTo = field.getFallTo();
			}
		}

		// �Z�����������o
		if (isMovingTime)
		{
			movingTimer += deltaTime;

			if (movingTimer > movingCoolTime)
			{
				field.moveCells(fieldMoveTo);
				isMovingTime = false;
				movingTimer = 0.0;

				// ��������̂��Ȃ��Ȃ�A��������悤�ɂ���
				if (!updatedField())
				{
					canDrop = true;
					combo = 0;
					//ret = obstructsMaked;
					sendingObstructCnt += obstructsMakedCnt;
					obstructsMakedCnt = 0;
				}
			}
		}

		// �I�W���}�𑗂鉉�o
		if (sendingObstructCnt > 0)
		{
			sendingObstructTimer += deltaTime;

			if (isSendObstruct)
			{
				isSendObstruct = false;
				sendingObstructCnt = 0;
				sendingObstructTimer = 0.0;
			}

			if (sendingObstructTimer > sendingObstructCoolTime)
			{
				isSendObstruct = true;
				sendingObstructTimer = 0.0;
			}
		}

		// �����ƕ\�����鉉�o
		if (state != BattleState::playing)
		{
			stateTimer += deltaTime;
		}
	}
}

void Player::draw(const PlayerData& data, Size cellSize) const
{
	const Point fieldPos = data.fieldPos;
	const FieldDrawMode drawMode = data.drawMode;
	const Size drawsize = field.getDrawsize();
	const Point textMoved = (Vec2(0, -1.5) * cellSize).asPoint();
	const Point fieldCenter = fieldPos + drawsize * cellSize / 2;
	const Character& chara = Character::getCharacters().at(data.characterNum);

	// Next�Z���C�L�����N�^�[�̕`��
	{
		const Point nextPos =
			drawMode == FieldDrawMode::Left ? fieldPos + Point(-2, 1) * cellSize :
			drawMode == FieldDrawMode::Right ? fieldPos + Point(drawsize.x + 2, 1) * cellSize :
			Point();
		const Point nextPosDiff =
			drawMode == FieldDrawMode::Left ? (Vec2(0.5, -0.5) * cellSize).asPoint() :
			drawMode == FieldDrawMode::Right ? (Vec2(-0.5, -0.5) * cellSize).asPoint() :
			Point();

		FontAsset(U"Text")(U"Next").drawAt(nextPos.movedBy(textMoved).movedBy(2, 3), ColorF(0.0, 0.4));
		FontAsset(U"Text")(U"Next").drawAt(nextPos.movedBy(textMoved), ColorF(0.2));
		Rect(Arg::center(nextPos), cellSize * 2).drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4));
		for (auto i : step(2, 4))
		{
			Rect(Arg::center(nextPos + nextPosDiff + i * Point(0, cellSize.y + 4)), cellSize).drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4));
		}
		getDropCellNotAdd(1).getTexture().resized(cellSize * 2).drawAt(nextPos);
		for (auto i : step(2, 4))
		{
			getDropCellNotAdd(i).getTexture().resized(cellSize).drawAt(nextPos + nextPosDiff + i * Point(0, cellSize.y + 4));
		}

		chara.texture.fitted(cellSize * 5).draw(Arg::topCenter = nextPos.movedBy(0, (cellSize.y + 4) * 5.5));
	}

	// �z�[���h�̕\��
	{
		const Point holdPos =
			drawMode == FieldDrawMode::Left ? fieldPos + Point(drawsize.x + 2, 1) * cellSize :
			drawMode == FieldDrawMode::Right ? fieldPos + Point(-2, 1) * cellSize :
			Point();
		const Texture hold = holdCell.getType() == CellType::Empty ? Cell::getTexture(CellType::None) : holdCell.getTexture();
		FontAsset(U"Text")(U"Hold").drawAt(holdPos.movedBy(textMoved).movedBy(2, 3), ColorF(0.0, 0.4));
		FontAsset(U"Text")(U"Hold").drawAt(holdPos.movedBy(textMoved), ColorF(0.2));
		Rect(Arg::center(holdPos), cellSize * 2).drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4));
		hold.resized(cellSize * 2).drawAt(holdPos);
	}

	// �t�B�[���h�̔w�i�Ƙg
	Rect(fieldPos - cellSize * Size(0, 1), (drawsize + Size(0, 1)) * cellSize)
		.drawShadow(Vec2(9, 15), 10.0, 10.0, ColorF(0.0, 0.4))
		.draw(fieldColor)
		.drawFrame(0.0, 10.0, ColorF(0.2));

	// �֎~�ꏊ�̕\��
	for (int32 x : step(drawsize.x))
	{
		Point pos = fieldPos + Point(x, 0) * cellSize;

		Cell::getTexture(CellType::No).resized(cellSize).draw(pos);
	}

	// �Z���̕\��
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

	// ���Ƃ��Z���̕`��
	{
		const Point dropCellPos(fieldPos + Point(cellSize.x * dropCellFieldX, -cellSize.y));

		getDropCellNotAdd(0).getTexture().resized(cellSize).draw(dropCellPos);
	}

	// �X�R�A
	{
		const Point pos(fieldCenter.x, fieldPos.y + drawsize.y * cellSize.y + 50);
		FontAsset(U"Text")(U"Score:{}"_fmt(score)).drawAt(pos.movedBy(2, 3), ColorF(0.0, 0.4));
		FontAsset(U"Text")(U"Score:{}"_fmt(score)).drawAt(pos, ColorF(0.25));
	}

	// �R���{
	if (combo > 0)
	{
		FontAsset(U"Combo")(U"{}"_fmt(combo)).drawAt(fieldCenter, ColorF(0.25, 0.75));
	}

	// �����̒m�点
	if (state != BattleState::playing)
	{
		const double e = EaseOutBounce(Min(stateTimer / stateCoolTime, 1.0));
		const Vec2 to(fieldCenter);
		const Vec2 from(fieldCenter.x, -100);
		const String text
			= state == BattleState::win ? U"Win"
			: state == BattleState::tie ? U"Tie"
			: state == BattleState::lose ? U"Lose"
			: U"----";
		FontAsset(U"Header")(text).drawAt(from.lerp(to, e), ColorF(0.2));
		//FontAsset(U"Header")(text).drawAt(to, ColorF(0.2));
	}
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

void Player::sentObstructs(double sent_obstructs)
{
	const Size size = field.getDrawsize();

	int32 obstructs = static_cast<int32>(ceil(sent_obstructs));

	obstructsSentSum += obstructs;
	for (auto i : step(size.x))
	{
		obstructsSentCntArray.at(i) += obstructs / size.x;
	}

	obstructs %= size.x;

	Array<int32> put(size.x, 0);
	for (auto i : step(size.x))
	{
		put.at(i) = i;
	}

	for (auto i : put.choice(obstructs))
	{
		++obstructsSentCntArray.at(i);
	}
}

void Player::makeObstruct()
{
	const auto size = field.getSize();
	const auto drawsize = field.getDrawsize();

	if (obstructsSentSum > 0)
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
	}
}
