
# pragma once
# include "Common.hpp"
# include "Cell.hpp"
# include "CellField.hpp"
# include "Player.hpp"

// ゲームシーン
class Battle : public MyApp::Scene
{
private:

	// セルの表示用大きさ
	//const Size cellDrawSize = Size(40, 40);
	const Size cellSize = Size(64, 64);

	// 負け待機時間
	double loseWaitTime = 10.0;

	// プレイヤーと、それぞれの操作キー
	Array<PlayerData> playerDatas = Array<PlayerData>();

public:

	// デバッグ用のPrintをするかどうか
	bool debugPrint = false;

	Battle(const InitData& init);

	void update() override;

	void draw() const override;
};
