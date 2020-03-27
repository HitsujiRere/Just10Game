
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerOperator.hpp"
# include "Character.hpp"

// プレイヤーのデータ
class PlayerData
{
public:

	std::shared_ptr<Player> playerPtr;
	std::shared_ptr<PlayerOperator> operaterPtr;
	Point fieldPos;
	FieldDrawMode drawMode;
	int32 characterNum;

	PlayerData();

	PlayerData(const PlayerData& another);

	PlayerData& operator=(const PlayerData& another);
};
