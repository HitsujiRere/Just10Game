
# pragma once

# include <Siv3D.hpp> // OpenSiv3D v0.4.2

// バージョン
const Vector3D<int32> Version(0, 3, 0);

// 戻るキー
const KeyGroup backKeys(KeyEscape, KeyX, KeyDelete, KeyBackspace);

// シーンの名前
enum class State
{
	// タイトル
	Title,

	// 遊び方
	HowTo,

	// プレイ
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

// a/bを切り上げしたものを返す
template<class T>
T divup(T a, T b)
{
	if (a % b == 0) { return a / b; }
	return a / b + (T)1;
}
