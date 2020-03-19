# include "PlayerData.hpp"

PlayerData::PlayerData()
{
	player = Player();
	keySet = PlayerKeySet();
	fieldPos = Point();
	drawMode = FieldDrawMode::Left;
}

PlayerData::PlayerData(Player _player, PlayerKeySet _keySet, Point _fieldPos, FieldDrawMode _drawMode)
	: player(_player)
	, keySet(_keySet)
	, fieldPos(_fieldPos)
	, drawMode(_drawMode)
{
}

PlayerData::PlayerData(const PlayerData& another)
	: player(another.player)
	, keySet(another.keySet)
	, fieldPos(another.fieldPos)
	, drawMode(another.drawMode)
{
}

PlayerData& PlayerData::operator=(const PlayerData& another)
{
	this->player = another.player;
	this->keySet = another.keySet;
	this->fieldPos = another.fieldPos;
	this->drawMode = another.drawMode;
	return *this;
}
