
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerKeySet.hpp"
# include "Character.hpp"

// プレイヤーのデータ
class PlayerData
{
public:
	Player player;
	PlayerKeySet keySet;
	Point fieldPos;
	FieldDrawMode drawMode;
	Character character;

	PlayerData();

	PlayerData(const PlayerData& another);

	PlayerData& operator=(const PlayerData& another);
};
