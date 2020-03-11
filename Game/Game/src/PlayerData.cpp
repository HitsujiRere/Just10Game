# include "PlayerData.hpp"

PlayerData::PlayerData()
{
	player = Player();
	keySet = PlayerKeySet();
	fieldPos = Point();
}

PlayerData::PlayerData(Player _player, PlayerKeySet _keySet, Point _fieldPos)
	: player(_player)
	, keySet(_keySet)
	, fieldPos(_fieldPos)
{
}

PlayerData::PlayerData(const PlayerData& another)
	: player(another.player)
	, keySet(another.keySet)
	, fieldPos(another.fieldPos)
{
}

PlayerData& PlayerData::operator=(const PlayerData& another)
{
	this->player = another.player;
	this->keySet = another.keySet;
	this->fieldPos = another.fieldPos;
	return *this;
}
