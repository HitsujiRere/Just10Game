
# pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.4.2

// バージョン
const Vector3D<int32> Version(0, 1, 0);

// 戻るキー
const KeyGroup backKeys(KeyEscape, KeyX);

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

	// プレイ回数
	int32 playTime = 0;
};

// シーン管理クラス
using MyApp = SceneManager<State, GameData>;
