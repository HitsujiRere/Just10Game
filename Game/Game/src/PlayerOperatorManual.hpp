
# pragma once

# include <Siv3D.hpp>
# include "PlayerOperator.hpp"

// プレイヤーのマニュアル操作
class PlayerOperatorManual : public PlayerOperator
{
private:

	KeyGroup KeysLeft = KeyGroup();
	KeyGroup KeysRight = KeyGroup();
	KeyGroup KeysDown = KeyGroup();
	KeyGroup KeysUp = KeyGroup();

	// 落とすセルの移動残り時間
	double dropCellTimer = 0.1;
	// 落とすセルの移動時間
	const double dropCellCoolTime = 0.1;

public:

	PlayerOperatorManual();

	PlayerOperatorManual(KeyGroup _KeysLeft, KeyGroup _KeysRight, KeyGroup _KeysDown, KeyGroup _KeysUp);

	PlayerOperatorManual& operator=(const PlayerOperatorManual& another);

	bool isMoveL() const override;

	bool isMoveR() const override;

	bool isHold() const override;

	bool isDrop() const override;

	bool isDecide() const override;
};
