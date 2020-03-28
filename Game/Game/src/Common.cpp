
# include "Common.hpp"

bool GameData::loadGameData(String path)
{
	BinaryReader reader(path);

	if (!reader)
	{
		return false;
	}

	bool ret = true;

	int32 readedLoadDataVer;
	reader.read(readedLoadDataVer);

	switch (readedLoadDataVer)
	{
	case 1:
		reader.read(highScore);
		reader.read(playTime);
		break;

	default:
		break;
	}

	return ret;
}

void GameData::saveGameData(String path) const
{
	BinaryWriter writter(path);

	if (!writter)
	{
		return;
	}

	const int32 loadGameDataVer = 1;
	writter.write(loadGameDataVer);

	writter.write(highScore);
	writter.write(playTime);
}

void GameData::loadSetting(String path)
{
	const INIData ini(path);

	if (!ini)
	{
		return;
	}

	this->debug = Parse<bool>(ini[U"Debug.isDebug"]);
	this->charaPath = ini[U"Chara.path"].split(',');
	this->charaResourcePath = ini[U"Chara.resource"].split(',');
}
