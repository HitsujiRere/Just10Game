
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerOperator.hpp"

// プレイヤーのマニュアル操作
class PlayerOperatorAuto : public PlayerOperator
{
private:

	// プレイヤー
	std::shared_ptr<Player> playerPtr;
	// バトル開始かどうか
	bool isBattleMode = false;

	// 行き先のX
	int32 destX = 0;

	KeyGroup KeysLeft = KeyGroup();
	KeyGroup KeysRight = KeyGroup();
	KeyGroup KeysDown = KeyGroup();
	KeyGroup KeysUp = KeyGroup();

	// 移動待機残り時間
	double moveTimer = 0.0;
	// 移動待機最大時間
	double moveCoolTime = 1.0;

public:

	PlayerOperatorAuto(KeyGroup _KeysLeft, KeyGroup _KeysRight, KeyGroup _KeysDown, KeyGroup _KeysUp, double _moveCoolTime = 1.0);

	PlayerOperatorAuto& operator=(const PlayerOperatorAuto& another);

	void update() override;

	void updateDestX(const CellField& field);

	bool isMoveL() const override;

	bool isMoveR() const override;

	bool isHold() const override;

	bool isDrop() const override;

	bool isDecide() const override;

	void setPlayer(std::shared_ptr<Player> _playerPtr);
};
