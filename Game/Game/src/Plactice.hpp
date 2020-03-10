
# pragma once
# include "Common.hpp"
# include "Cell.hpp"
# include "CellField.hpp"
# include "Player.hpp"

// ゲームシーン
class Plactice : public MyApp::Scene
{
private:

	// セルの表示用大きさ
	//const Size cellDrawSize = Size(40, 40);
	const Size cellSize = Size(64, 64);

	// プレイヤーと、それぞれの操作キー
	PlayerData playerData = PlayerData();

	// 終了したかどうか
	bool isFinished = false;

	// 終了演出残り時間
	double backTimer = 0.0;
	// 終了時間時間
	double backTime = 10.0;

public:

	// デバッグ用のPrintをするかどうか
	bool debugPrint = false;

	Plactice(const InitData& init);

	void update() override;

	void draw() const override;
};
