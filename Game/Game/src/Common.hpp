
# pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.4.2

// シーンの名前
enum class State
{
	Title,

	HowTo,

	Battle
};

// ゲームデータ
struct GameData
{
	// ハイスコア
	int32 highScore = 0;
};

// シーン管理クラス
using MyApp = SceneManager<State, GameData>;
