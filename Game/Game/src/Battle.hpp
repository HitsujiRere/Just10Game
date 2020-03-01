
# pragma once
# include "Common.hpp"
# include "Cell.hpp"
# include "CellField.hpp"

// ゲームシーン
class Battle : public MyApp::Scene
{
private:

	// セルの取りうる最大数
	const int32 cellMaxNumber = 5;

	// セルの表示用大きさ
	//const Size cellDrawSize = Size(40, 40);
	const Size cellDrawSize = Size(64, 64);

	// フィールドの大きさ
	const Size fieldSize = Size(6, 6);
	// フィールドの左上の位置
	const Point fieldPos = Point(cellDrawSize * Size(1, 2));
	// フィールド
	CellField field = CellField(fieldSize);
	// フィールドのセルのJust10の要素となっている回数
	Grid<int32> just10Times = Grid<int32>(fieldSize);

	// 落とすセル
	Cell dropCell = Cell::getRandomCell(cellMaxNumber);
	// 落とすフィールドのx
	int32 dropCellFieldX = 0;
	// セルの移動先
	Grid<Point> fieldMoveTo = Grid<Point>(fieldSize);

	// 操作できるかどうか
	bool canOperate = true;

	// Just10消去待機時間かどうか
	bool isDeletingTime = false;
	// Just10消去待機残り時間
	double deletingTimer = 0.0;
	// Just10消去待機時間
	double deletingCoolTime = 1.0;

	// セル落下待機時間かどうか
	bool isFallingTime = false;
	// Just10消去待機残り時間
	double fallingTimer = 0.0;
	// Just10消去待機時間
	double fallingCoolTime = 1.0;

	// スコア
	int32 m_score = 0;

	// fieldJust10Timesなどの更新
	// 返り値は消えるものがあるかどうか
	bool updatedField();

public:

	// デバッグ用のPrintをするかどうか
	bool debugPrint = true;

	Battle(const InitData& init);

	void update() override;

	void draw() const override;
};
