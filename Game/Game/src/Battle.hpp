
# pragma once

# include "Common.hpp"
# include "Cell.hpp"
# include "CellField.hpp"
# include "Player.hpp"
# include "PlayerKeySet.hpp"
# include "PlayerData.hpp"

// バトルの種類
enum class BattleType
{
	// 1人であそぶ
	By1,

	// 2人でたたかう
	By2,
};

// ゲームシーン
class Battle : public MyApp::Scene
{
private:

	// セルの表示用大きさ
	//const Size cellDrawSize = Size(40, 40);
	const Size cellSize = Size(64, 64);

	// プレイヤーと、それぞれの操作キー
	Array<PlayerData> playerDatas = Array<PlayerData>();

	// 終了したかどうか
	bool isFinished = false;

	// 終了演出残り時間
	double backTimer = 0.0;
	// 終了時間時間
	double backTime = 10.0;

public:

	// バトルの種類
	static BattleType battleType;

	// デバッグ用のPrintをするかどうか
	bool debugPrint = false;

	Battle(const InitData& init);

	void update() override;

	void draw() const override;
};
