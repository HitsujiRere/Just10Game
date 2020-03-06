#pragma once

# include "Common.hpp"
# include "Cell.hpp"
# include "CellField.hpp"

// 試合の状態
enum class BattleState
{
	// 試合中
	playing,
	// 勝ち
	win,
	// 負け
	lose,
	// 引き分け
	tie,
};

// プレイヤー
class Player
{
private:
	// 落とすセルの移動時間
	const double dropCellCoolTime = 0.1;
	// 落とすセルの移動残り時間
	double dropCellTimer = 0.1;
	// 落とすセルの1ループ
	Array<Cell> dropCells1LoopCells;
	// 落とすセルの次のスタック
	Array<Cell> dropCells = Array<Cell>();

	// fieldJust10Timesなどの更新
	// 返り値は消えるものがあるかどうか
	bool updatedField();

public:
	Player(Array<int32> _dropCells1LoopNum = { 0, 1, 1, 1, 1, 1 });

	// デバッグ用のPrintをするかどうか
	bool debugPrint = false;

	// フィールドの大きさ
	const Size fieldSize = Size(6, 12);
	// フィールド
	CellField field = CellField(fieldSize);
	// フィールドのセルのJust10の要素となっている回数
	Grid<int32> just10Times = Grid<int32>(fieldSize);
	// セルの移動先
	Grid<Point> fieldMoveTo = Grid<Point>(fieldSize);

	// 落とすフィールドのx
	int32 dropCellFieldX = 0;
	// 落とすセルの1ループのNumberの数
	const Array<int32> dropCells1LoopNum = Array<int32>
	{
		0, 1, 1, 1, 1, 1,
	};

	// ホールドセル
	Cell holdCell = Cell((int32)CellTypeNumber::Empty);

	// 操作できるかどうか
	bool canOperate = true;
	// 落とすセルを落下できるかどうか
	bool canDrop = true;

	// Just10消去待機時間かどうか
	bool isDeletingTime = false;
	// Just10消去待機残り時間
	double deletingTimer = 0.0;
	// Just10消去待機時間
	const double deletingCoolTime = 1.0;

	// セル落下待機時間かどうか
	bool isFallingTime = false;
	// Just10消去待機残り時間
	double fallingTimer = 0.0;
	// Just10消去待機時間
	const double fallingCoolTime = 0.5;

	// スコア
	int32 m_score = 0;

	// 勝敗
	int32 state = (int32)BattleState::playing;

	// 落とすセルを取得する
	Cell& getDropCell(int32 n);

	// 落とすセルを取得する
	// numを超えても追加しない
	const Cell& getDropCellConst(int32 num) const;

	void update();

	void draw(Point fieldPos, Size cellDrawSize) const;
};
