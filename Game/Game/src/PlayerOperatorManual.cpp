
# include "PlayerOperatorManual.hpp"

PlayerOperatorManual::PlayerOperatorManual()
{
}

PlayerOperatorManual::PlayerOperatorManual(KeyGroup _KeysLeft, KeyGroup _KeysRight, KeyGroup _KeysDown, KeyGroup _KeysUp)
	: KeysLeft(_KeysLeft)
	, KeysRight(_KeysRight)
	, KeysDown(_KeysDown)
	, KeysUp(_KeysUp)
{
}

PlayerOperatorManual& PlayerOperatorManual::operator=(const PlayerOperatorManual& another)
{
	this->KeysLeft = another.KeysLeft;
	this->KeysRight = another.KeysRight;
	this->KeysDown = another.KeysDown;
	this->KeysUp = another.KeysUp;

	return *this;
}

bool PlayerOperatorManual::isMoveL() const
{
	return KeysLeft.down();
}

bool PlayerOperatorManual::isMoveR() const
{
	return KeysRight.down();
}

bool PlayerOperatorManual::isHold() const
{
	return KeysUp.down();
}

bool PlayerOperatorManual::isDrop() const
{
	return KeysDown.down();
}

bool PlayerOperatorManual::isDecide() const
{
	return KeysUp.down() || KeysDown.down();
}
