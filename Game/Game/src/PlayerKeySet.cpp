
# include "PlayerKeySet.hpp"

PlayerKeySet::PlayerKeySet()
{
}

PlayerKeySet::PlayerKeySet(KeyGroup _KeysLeft, KeyGroup _KeysRight, KeyGroup _KeysDown, KeyGroup _KeysUp)
	: KeysLeft(_KeysLeft)
	, KeysRight(_KeysRight)
	, KeysDown(_KeysDown)
	, KeysUp(_KeysUp)
{
}

PlayerKeySet& PlayerKeySet::operator=(const PlayerKeySet& another)
{
	this->KeysLeft = another.KeysLeft;
	this->KeysRight = another.KeysRight;
	this->KeysDown = another.KeysDown;
	this->KeysUp = another.KeysUp;

	return *this;
}
