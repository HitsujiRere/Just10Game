
# pragma once

# include <Siv3D.hpp> // OpenSiv3D v0.4.2


// バージョン
const Vector3D<int32> Version(0, 5, 2);

// 戻るキー
const KeyGroup KeysBack(KeyEscape, KeyX, KeyDelete, KeyBackspace);

// シーンの名前
enum class State
{
	// タイトル
	Title,

	// 遊び方
	HowTo,

	// プレイ設定
	BattleSet,

	// プレイ
	Battle,
};

// バトルのプレイヤーの人数
enum class PlayerCount
{
	// 1人であそぶ
	By1 = 1,

	// 2人でたたかう
	By2 = 2,
};

// ゲームデータ
struct GameData
{
	// ハイスコア
	int32 highScore = 0;

	// プレイ回数
	int32 play_time = 0;

	// デバッグのプリントの可否
	bool debugPrint = false;

	// プレイヤーの人数
	PlayerCount playerCnt;

	// ゲームデータを指定ファイルからロードする
	// 返り値は、最新バージョンのGameDataがあったかどうか
	bool loadGameData(String path);

	// ゲームデータを指定ファイルにセーブする
	void saveGameData(String path) const;
};

// シーン管理クラス
using MyApp = SceneManager<State, GameData>;
