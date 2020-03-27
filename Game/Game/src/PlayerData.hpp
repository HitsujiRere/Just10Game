
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerOperator.hpp"
# include "Character.hpp"

// �v���C���[�̃f�[�^
class PlayerData
{
public:

	Player player;
	std::shared_ptr<PlayerOperator> operaterPtr;
	Point fieldPos;
	FieldDrawMode drawMode;
	int32 characterNum;

	PlayerData();

	PlayerData(const PlayerData& another);

	PlayerData& operator=(const PlayerData& another);
};
