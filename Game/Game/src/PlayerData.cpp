
# include "PlayerData.hpp"

PlayerData::PlayerData()
{
	playerPtr.reset();
	operaterPtr.reset();
	fieldPos = Point();
	drawMode = FieldDrawMode::Left;
	characterNum = 0;
}

PlayerData::PlayerData(const PlayerData& another)
	: playerPtr(another.playerPtr)
	, operaterPtr(another.operaterPtr)
	, fieldPos(another.fieldPos)
	, drawMode(another.drawMode)
	, characterNum(another.characterNum)
{
}

PlayerData& PlayerData::operator=(const PlayerData& another)
{
	this->playerPtr = another.playerPtr;
	this->operaterPtr = another.operaterPtr;
	this->fieldPos = another.fieldPos;
	this->drawMode = another.drawMode;
	this->characterNum = another.characterNum;

	return *this;
}
