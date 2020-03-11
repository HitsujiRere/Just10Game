
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerKeySet.hpp"

// プレイヤーのデータ
class PlayerData
{
public:
	Player player;
	PlayerKeySet keySet;
	Point fieldPos;

	PlayerData();

	PlayerData(Player _player, PlayerKeySet _keySet, Point _fieldPos);

	PlayerData(const PlayerData& another);

	PlayerData& operator=(const PlayerData& another);
};
