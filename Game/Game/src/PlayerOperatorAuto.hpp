
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerOperator.hpp"

// �v���C���[�̃}�j���A������
class PlayerOperatorAuto : public PlayerOperator
{
private:

	// �v���C���[
	std::unique_ptr<Player> playerPtr;
	// �o�g���J�n���ǂ���
	bool isStartBattle = false;

	// �s�����X
	int32 destX = 0;

	// �ړ��ҋ@�c�莞��
	double moveTimer = 0.0;
	// �ړ��ҋ@�ő厞��
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
