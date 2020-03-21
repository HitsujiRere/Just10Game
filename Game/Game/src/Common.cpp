
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
	case 2:
		reader.read(highScore);
		reader.read(play_time);
		reader.read(debugPrint);
		break;

	case 1:
		reader.read(highScore);
		reader.read(play_time);
		break;

	default:
		ret = false;
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

	const int32 loadGameDataVer = 2;
	writter.write(loadGameDataVer);

	writter.write(highScore);
	writter.write(play_time);
	writter.write(debugPrint);
}
