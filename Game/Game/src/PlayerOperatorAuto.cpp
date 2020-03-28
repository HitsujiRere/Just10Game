
# include "PlayerOperatorAuto.hpp"

PlayerOperatorAuto::PlayerOperatorAuto(KeyGroup _KeysLeft, KeyGroup _KeysRight, KeyGroup _KeysDown, KeyGroup _KeysUp, double _moveCoolTime)
	: playerPtr()
	, KeysLeft(_KeysLeft)
	, KeysRight(_KeysRight)
	, KeysDown(_KeysDown)
	, KeysUp(_KeysUp)
	, moveCoolTime(_moveCoolTime)
{
}

PlayerOperatorAuto& PlayerOperatorAuto::operator=(const PlayerOperatorAuto& another)
{
	*this->playerPtr = *another.playerPtr;
	this->isBattleMode = another.isBattleMode;

	this->destX = another.destX;

	this->KeysLeft = another.KeysLeft;
	this->KeysRight = another.KeysRight;
	this->KeysDown = another.KeysDown;
	this->KeysUp = another.KeysUp;

	this->moveTimer = another.moveTimer;
	this->moveCoolTime = another.moveCoolTime;

	return *this;
}

void PlayerOperatorAuto::update()
{
	if (isBattleMode)
	{
		const auto& deltaTime = Scene::DeltaTime();

		updateDestX(playerPtr->field);

		if (moveTimer > moveCoolTime)
		{
			moveTimer = 0.0;
		}
		else
		{
			moveTimer += deltaTime;
		}
	}
}

void PlayerOperatorAuto::updateDestX(const CellField& field)
{
	int32 maxTopY = 0, xOfMaxTopY = 0;
	for (auto x : step(field.getDrawsize().x))
	{
		int32 topY = field.getDrawsize().y;
		for (auto y : step(field.getDrawsize().y))
		{
			if (field.getField().at(y, x).getType() != CellType::Empty)
			{
				topY = y;
				break;
			}
		}

		//Print << U"x = {}, topY = {}"_fmt(x, topY);

		if (maxTopY < topY)
		{
			maxTopY = topY;
			xOfMaxTopY = x;
		}
	}

	destX = xOfMaxTopY;
}

bool PlayerOperatorAuto::isMoveL() const
{
	return (isBattleMode && (moveTimer > moveCoolTime) && (destX < playerPtr->dropCellFieldX)) || (KeysLeft.down());
}

bool PlayerOperatorAuto::isMoveR() const
{
	return (isBattleMode && (moveTimer > moveCoolTime) && (destX > playerPtr->dropCellFieldX)) || (KeysRight.down());
}

bool PlayerOperatorAuto::isHold() const
{
	return false;
}

bool PlayerOperatorAuto::isDrop() const
{
	return isBattleMode && (moveTimer > moveCoolTime) && (destX == playerPtr->dropCellFieldX);
}

bool PlayerOperatorAuto::isDecide() const
{
	return KeysUp.down() || KeysDown.down();
}

void PlayerOperatorAuto::setPlayer(std::shared_ptr<Player> _playerPtr)
{
	playerPtr = _playerPtr;
	isBattleMode = true;
}
