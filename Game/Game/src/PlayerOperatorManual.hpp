
# pragma once

# include <Siv3D.hpp>
# include "PlayerOperator.hpp"

// �v���C���[�̃}�j���A������
class PlayerOperatorManual : public PlayerOperator
{
private:

	KeyGroup KeysLeft = KeyGroup();
	KeyGroup KeysRight = KeyGroup();
	KeyGroup KeysDown = KeyGroup();
	KeyGroup KeysUp = KeyGroup();

	// ���Ƃ��Z���̈ړ��c�莞��
	double dropCellTimer = 0.1;
	// ���Ƃ��Z���̈ړ�����
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
