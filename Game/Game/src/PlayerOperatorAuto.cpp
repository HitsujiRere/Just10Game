
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
	//Print << U"PlayerOperatorAuto::update() begin";

	Stopwatch stopwach;

	stopwach.start();

	if (isStartBattle)
	{
		const auto& deltaTime = Scene::DeltaTime();

		updateDestX(playerPtr->field);

		//Print << U"destX = {}"_fmt(destX);

		if (moveTimer > moveCoolTime)
		{
			moveTimer = 0.0;
		}
		else
		{
			moveTimer += deltaTime;
		}

		//Print << U"moveTimer = {}"_fmt(moveTimer);
	}

	stopwach.pause();

	if (stopwach.ms() > 0)
		Print << U"time = {}"_fmt(stopwach.ms());

	//Print << U"PlayerOperatorAuto::update() end";
}

void PlayerOperatorAuto::updateDestX(const CellField& field)
{
	int32 minTopY = 0, xOfMinTopY = 0;
	for (auto x : step(field.getSize().x))
	{
		int32 topY = field.getSize().y;
		for (auto y : step(field.getSize().y))
		{
			if (field.getField().at(y, x).getType() != CellType::Empty)
			{
				topY = y;
				break;
			}
		}

		if (minTopY > topY)
		{
			minTopY = topY;
			xOfMinTopY = x;
		}
	}

	destX = xOfMinTopY;
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

void PlayerOperatorAuto::setPlayer(Player& _player)
{
	playerPtr = std::unique_ptr<Player>(&_player);
	isStartBattle = true;
}
