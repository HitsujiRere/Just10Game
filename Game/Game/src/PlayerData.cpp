# include "PlayerData.hpp"

PlayerData::PlayerData()
{
	player = Player();
	keySet = PlayerKeySet();
	fieldPos = Point();
	drawMode = FieldDrawMode::Left;
	character = Character();
}

PlayerData::PlayerData(const PlayerData& another)
	: player(another.player)
	, keySet(another.keySet)
	, fieldPos(another.fieldPos)
	, drawMode(another.drawMode)
	, character(another.character)
{
}

PlayerData& PlayerData::operator=(const PlayerData& another)
{
	this->player = another.player;
	this->keySet = another.keySet;
	this->fieldPos = another.fieldPos;
	this->drawMode = another.drawMode;
	this->character = another.character;

	return *this;
}
