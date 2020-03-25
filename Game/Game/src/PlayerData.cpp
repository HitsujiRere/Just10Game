# include "PlayerData.hpp"

PlayerData::PlayerData()
{
	player = Player();
	operaterPtr.reset();
	fieldPos = Point();
	drawMode = FieldDrawMode::Left;
	characterNum = 0;
}

PlayerData::PlayerData(const PlayerData& another)
	: player(another.player)
	, operaterPtr(another.operaterPtr)
	, fieldPos(another.fieldPos)
	, drawMode(another.drawMode)
	, characterNum(another.characterNum)
{
}

PlayerData& PlayerData::operator=(const PlayerData& another)
{
	this->player = another.player;
	this->operaterPtr = another.operaterPtr;
	this->fieldPos = another.fieldPos;
	this->drawMode = another.drawMode;
	this->characterNum = another.characterNum;

	return *this;
}
