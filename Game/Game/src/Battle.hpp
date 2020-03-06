
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

	// フィールドの左上の位置
	const Point fieldPos = Point(cellSize * Size(4, 2));

	// 負け演出残り時間
	double loseTimer = 0.0;
	// 負け演出時間
	const double loseCoolTime = 2.0;
	// 負け待機時間
	double loseWaitTime = 10.0;

	// 操作プレイヤー
	Player player = Player();

public:

	// デバッグ用のPrintをするかどうか
	bool debugPrint = false;

	Battle(const InitData& init);

	void update() override;

	void draw() const override;
};
