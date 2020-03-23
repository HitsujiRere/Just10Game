
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerKeySet.hpp"
# include "Character.hpp"

// �v���C���[�̃f�[�^
class PlayerData
{
public:
	Player player;
	PlayerKeySet keySet;
	Point fieldPos;
	FieldDrawMode drawMode;
	int32 characterNum;

	PlayerData();

	PlayerData(const PlayerData& another);

	PlayerData& operator=(const PlayerData& another);
};
