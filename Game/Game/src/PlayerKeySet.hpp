
#pragma once

# include <Siv3D.hpp>

// プレイヤーのキーセット
class PlayerKeySet
{
public:
	KeyGroup moveL = KeyGroup();
	KeyGroup moveR = KeyGroup();
	KeyGroup drop = KeyGroup();
	KeyGroup hold = KeyGroup();
	KeyGroup changeCell = KeyGroup();
	KeyGroup toRandom = KeyGroup();
	KeyGroup toEmpty = KeyGroup();

	PlayerKeySet();

	PlayerKeySet(KeyGroup _moveL, KeyGroup _moveR, KeyGroup _drop, KeyGroup _hold,
		KeyGroup _changeCell = KeyGroup(), KeyGroup _toRandom = KeyGroup(),
		KeyGroup _toEmpty = KeyGroup());

	PlayerKeySet& operator=(const PlayerKeySet& another);
};
