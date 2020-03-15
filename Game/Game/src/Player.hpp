
#pragma once

# include "Common.hpp"
# include "Cell.hpp"
# include "CellField.hpp"
# include "PlayerKeySet.hpp"

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
	// 落とすセルの1ループ
	Array<Cell> dropCells1LoopCells;
	// 落とすセルの次のスタック
	Array<Cell> dropCellStack = Array<Cell>();

	// fieldJust10Timesなどの更新
	// 返り値は消えるものがあるかどうか
	bool updatedField();

public:
	Player(Array<int32> _dropCells1LoopNum = { 0, 1, 1, 1, 1, 1 });
	//Player(const Player& another);
	//Player(Player&& another);

	Player& operator=(const Player& another);

	// フィールドの大きさ
	const Size fieldSize = Size(6, 12);
	// フィールド
	CellField field = CellField(fieldSize);
	// フィールドのセルのJust10の要素となっている回数
	Grid<int32> deleteField = Grid<int32>(fieldSize);
	// セルの移動先
	Grid<Point> fieldMoveTo = Grid<Point>(fieldSize);

	// フィールド（背景）の色
	Color fieldColor = ColorF(0.8);

	// 落とすセルの移動時間
	const double dropCellCoolTime = 0.1;
	// 落とすセルの移動残り時間
	double dropCellTimer = 0.1;
	// 落とすフィールドのx
	int32 dropCellFieldX = 0;
	// 落とすセルの1ループのNumberの数
	Array<int32> dropCells1LoopNum = Array<int32>
	{
		0, 1, 1, 1, 1, 1,
	};

	// ホールドセル
	Cell holdCell = Cell(CellType::Empty);

	// 操作できるかどうか
	bool canOperate = true;
	// 落とすセルを落下できるかどうか
	bool canDrop = true;

	// Just10消去待機時間かどうか
	bool isDeletingTime = false;
	// Just10消去待機残り時間
	double deletingTimer = 0.0;
	// Just10消去待機最大時間
	const double deletingCoolTime = 1.0;

	// セル落下待機時間かどうか
	bool isFallingTime = false;
	// Just10消去待機残り時間
	double fallingTimer = 0.0;
	// Just10消去待機時間
	const double fallingCoolTime = 0.5;

	// セル押上待機時間かどうか
	bool isPushedTime = false;
	// セル押上待機残り時間
	double pushedTimer = 0.0;
	// セル押上待機最大時間
	const double pushedCoolTime = 0.5;

	// 負け演出残り時間
	double loseTimer = 0.0;
	// 負け演出最大時間
	const double loseCoolTime = 2.0;

	// スコア
	int32 score = 0;
	// スコア計算関数
	std::function<int32(int32 dc, int32 combo)> scoreFunc
		= [](int32 d, int32 c) {return (int32)(d * (c / 2.0 + 1.0)); };
	// コンボ回数
	int32 combo = 0;

	// 作ったオジャマ
	int32 obstructsMaked = 0;
	// 保留中の受けたオジャマの数
	int32 obstructsSentSum = 0;
	// 保留中の受けたオジャマの列別数
	Array<int32> obstructsSentCntArray = Array<int32>(fieldSize.x);
	// 表示保留中のオジャマ
	Grid<Cell> obstructsCntDrawField = Grid<Cell>(fieldSize);

	// 与えるダメージ割合
	double atkRate = 1.0;
	// 与えられるダメージ割合
	double defRate = 1.0;

	// 勝敗
	BattleState state = BattleState::playing;

	// 落とすセルを作成する
	void makeDropCells(int32 min);

	// 落とすセルを取得する
	// numがdropCellStackのサイズを超えていたら追加する
	inline const Cell& getDropCell(int32 num)
	{
		makeDropCells(num);

		return dropCellStack.at(num);
	}

	// 落とすセルを取得する
	inline const Cell& getDropCellConst(int32 num) const
	{
		return dropCellStack.at(num);
	}

	// 更新する
	// 返り値は作成したオジャマ
	int32 update(PlayerKeySet keySet);

	void draw(Point fieldPos, Size cellDrawSize) const;

	// オジャマを送る
	void sendObstructs(double sent_obstructs);
};
