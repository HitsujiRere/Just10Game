
# include "PlayerKeySet.hpp"

PlayerKeySet::PlayerKeySet()
{
}

PlayerKeySet::PlayerKeySet(KeyGroup _moveL, KeyGroup _moveR, KeyGroup _drop, KeyGroup _hold,
	KeyGroup _changeCell, KeyGroup _toRandom, KeyGroup _toEmpty)
	: moveL(_moveL)
	, moveR(_moveR)
	, drop(_drop)
	, hold(_hold)
	, changeCell(_changeCell)
	, toRandom(_toRandom)
	, toEmpty(_toEmpty)
{
}

PlayerKeySet& PlayerKeySet::operator=(const PlayerKeySet& another)
{
	this->moveL = another.moveL;
	this->moveR = another.moveR;
	this->drop = another.drop;
	this->hold = another.hold;
	this->changeCell = another.changeCell;
	this->toRandom = another.toRandom;
	this->toEmpty = another.toEmpty;

	return *this;
}
