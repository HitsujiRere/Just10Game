
# include "PlayerOperatorAuto.hpp"

PlayerOperatorAuto::PlayerOperatorAuto(double _moveCoolTime)
	: playerPtr()
	, moveCoolTime(_moveCoolTime)
{
}

PlayerOperatorAuto& PlayerOperatorAuto::operator=(const PlayerOperatorAuto& another)
{
	*this->playerPtr = *another.playerPtr;
	this->isStartBattle = another.isStartBattle;

	this->destX = another.destX;

	this->moveTimer = another.moveTimer;
	this->moveCoolTime = another.moveCoolTime;

	return *this;
}

void PlayerOperatorAuto::update()
{
	if (isStartBattle)
	{
		const auto& deltaTime = Scene::DeltaTime();

		updateDestX(playerPtr->field);

		Print << U"destX = {}"_fmt(destX);

		//Print << U"playerPtr->field";
		//Print << playerPtr->field.getField();

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
	if (isStartBattle && (moveTimer > moveCoolTime) && (destX < playerPtr->dropCellFieldX))
		Print << U"move left!";
	return isStartBattle && (moveTimer > moveCoolTime) && (destX < playerPtr->dropCellFieldX);
}

bool PlayerOperatorAuto::isMoveR() const
{
	if (isStartBattle && (moveTimer > moveCoolTime) && (destX > playerPtr->dropCellFieldX))
		Print << U"move right!";
	return isStartBattle && (moveTimer > moveCoolTime) && (destX > playerPtr->dropCellFieldX);
}

bool PlayerOperatorAuto::isHold() const
{
	return false;
}

bool PlayerOperatorAuto::isDrop() const
{
	if (isStartBattle && (moveTimer > moveCoolTime) && (destX == playerPtr->dropCellFieldX))
		Print << U"drop!";
	return isStartBattle && (moveTimer > moveCoolTime) && (destX == playerPtr->dropCellFieldX);
}

bool PlayerOperatorAuto::isDecide() const
{
	return false;
}

void PlayerOperatorAuto::setPlayer(std::shared_ptr<Player> _playerPtr)
{
	playerPtr = _playerPtr;
	isStartBattle = true;
}
