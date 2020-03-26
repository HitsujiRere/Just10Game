
# include "PlayerOperatorAuto.hpp"

PlayerOperatorAuto::PlayerOperatorAuto(double _moveCoolTime)
	: moveCoolTime(_moveCoolTime)
{
}

PlayerOperatorAuto& PlayerOperatorAuto::operator=(const PlayerOperatorAuto& another)
{
	return *this;
}

/*
void PlayerOperatorAuto::update(const Player player)
{
}

/**
void PlayerOperatorAuto::update(const Player& player)
{
	const auto deltaTime = Scene::DeltaTime();

	updateDestX(player.field);

	playerX = player.dropCellFieldX;

	if (moveTimer > moveCoolTime)
	{
		moveTimer = 0.0;
	}
	else
	{
		moveTimer += deltaTime;
	}
}
/**/

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
			}
		}

		if (minTopY > topY)
		{
			minTopY = topY;
			xOfMinTopY = x;
		}
	}

	xOfMinTopY = destX;
}

bool PlayerOperatorAuto::isMoveL() const
{
	return (moveTimer > moveCoolTime) && (destX < playerX);
}

bool PlayerOperatorAuto::isMoveR() const
{
	return (moveTimer > moveCoolTime) && (destX > playerX);
}

bool PlayerOperatorAuto::isHold() const
{
	return false;
}

bool PlayerOperatorAuto::isDrop() const
{
	return (moveTimer > moveCoolTime) && (destX == playerX);
}

bool PlayerOperatorAuto::isDecide() const
{
	return false;
}
