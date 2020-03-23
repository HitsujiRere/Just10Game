
#pragma once

# include <Siv3D.hpp>

// �v���C���[�̃L�[�Z�b�g
class PlayerKeySet
{
public:
	KeyGroup KeysLeft = KeyGroup();
	KeyGroup KeysRight = KeyGroup();
	KeyGroup KeysDown = KeyGroup();
	KeyGroup KeysUp = KeyGroup();

	PlayerKeySet();

	PlayerKeySet(KeyGroup _KeysLeft, KeyGroup _KeysRight, KeyGroup _KeysDown, KeyGroup _KeysUp);

	PlayerKeySet& operator=(const PlayerKeySet& another);
};
