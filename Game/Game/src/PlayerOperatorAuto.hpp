
# pragma once

# include <Siv3D.hpp>
# include "Player.hpp"
# include "PlayerOperator.hpp"

// �v���C���[�̃}�j���A������
class PlayerOperatorAuto : public PlayerOperator
{
private:

	// �v���C���[
	std::shared_ptr<Player> playerPtr;
	// �o�g���J�n���ǂ���
	bool isBattleMode = false;

	// �s�����X
	int32 destX = 0;

	KeyGroup KeysLeft = KeyGroup();
	KeyGroup KeysRight = KeyGroup();
	KeyGroup KeysDown = KeyGroup();
	KeyGroup KeysUp = KeyGroup();

	// �ړ��ҋ@�c�莞��
	double moveTimer = 0.0;
	// �ړ��ҋ@�ő厞��
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
