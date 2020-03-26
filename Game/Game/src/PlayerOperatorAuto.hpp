
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerOperator.hpp"

// プレイヤーのマニュアル操作
class PlayerOperatorAuto : public PlayerOperator
{
private:

	// 現在のX
	int32 playerX = 0;

	// 行き先のX
	int32 destX = 0;

	// 移動待機残り時間
	double moveTimer = 0.0;
	// 移動待機最大時間
	double moveCoolTime = 3.0;

public:

	PlayerOperatorAuto(double _moveCoolTime = 3.0);

	PlayerOperatorAuto& operator=(const PlayerOperatorAuto& another);

	//void update(const Player player) override;

	//void update(const Player& player) override;

	void updateDestX(const CellField& field);

	bool isMoveL() const override;

	bool isMoveR() const override;

	bool isHold() const override;

	bool isDrop() const override;

	bool isDecide() const override;
};
