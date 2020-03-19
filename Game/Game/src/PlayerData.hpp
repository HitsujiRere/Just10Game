
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
	FieldDrawMode drawMode;

	PlayerData();

	PlayerData(Player _player, PlayerKeySet _keySet, Point _fieldPos, FieldDrawMode _drawMode);

	PlayerData(const PlayerData& another);

	PlayerData& operator=(const PlayerData& another);
};
