
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerOperator.hpp"

// �v���C���[�̃}�j���A������
class PlayerOperatorAuto : public PlayerOperator
{
private:

	// ���݂�X
	int32 playerX = 0;

	// �s�����X
	int32 destX = 0;

	// �ړ��ҋ@�c�莞��
	double moveTimer = 0.0;
	// �ړ��ҋ@�ő厞��
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
