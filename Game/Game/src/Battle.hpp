
# pragma once

# include "Common.hpp"
# include "Cell.hpp"
# include "CellField.hpp"
# include "Player.hpp"
# include "PlayerKeySet.hpp"
# include "PlayerData.hpp"

// ゲームシーン
class Battle : public MyApp::Scene
{
private:

	// セルの表示用大きさ
	const Size cellSize = Size(64, 64);

	// 終了したかどうか
	bool isFinished = false;

	// 終了演出残り時間
	double backTimer = 0.0;
	// 終了時間時間
	double backTime = 10.0;

public:

	// プレイヤーと、それぞれの操作キー
	static Array<PlayerData> playerDatas;

	Battle(const InitData& init);

	void update() override;

	void draw() const override;
};
