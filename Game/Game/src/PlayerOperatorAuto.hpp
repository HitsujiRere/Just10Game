
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerOperator.hpp"

// プレイヤーのマニュアル操作
class PlayerOperatorAuto : public PlayerOperator
{
private:

	// プレイヤー
	std::unique_ptr<Player> playerPtr;
	// バトル開始かどうか
	bool isStartBattle = false;

	// 行き先のX
	int32 destX = 0;

	// 移動待機残り時間
	double moveTimer = 0.0;
	// 移動待機最大時間
	double moveCoolTime = 3.0;

public:

	PlayerOperatorAuto(double _moveCoolTime = 3.0);

	PlayerOperatorAuto& operator=(const PlayerOperatorAuto& another);

	void update() override;

	void updateDestX(const CellField& field);

	bool isMoveL() const override;

	bool isMoveR() const override;

	bool isHold() const override;

	bool isDrop() const override;

	bool isDecide() const override;

	void setPlayer(Player& _player);
};
