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

// プレイヤーのキーセット
class PlayerKeySet
{
public:
	KeyGroup moveL = KeyGroup();
	KeyGroup moveR = KeyGroup();
	KeyGroup drop = KeyGroup();
	KeyGroup hold = KeyGroup();
	KeyGroup changeCell = KeyGroup();
	KeyGroup toRandom = KeyGroup();
	KeyGroup toEmpty = KeyGroup();

	PlayerKeySet()
	{
	}

	PlayerKeySet(KeyGroup _moveL, KeyGroup _moveR, KeyGroup _drop, KeyGroup _hold,
		KeyGroup _changeCell = KeyGroup(), KeyGroup _toRandom = KeyGroup(),
		KeyGroup _toEmpty = KeyGroup())
		: moveL(_moveL)
		, moveR(_moveR)
		, drop(_drop)
		, hold(_hold)
		, changeCell(_changeCell)
		, toRandom(_toRandom)
		, toEmpty(_toEmpty)
	{
	}

	PlayerKeySet& operator=(const PlayerKeySet& another)
	{
		this->moveL = another.moveL;
		this->moveR = another.moveR;
		this->drop = another.drop;
		this->hold = another.hold;
		this->changeCell = another.changeCell;
		this->toRandom = another.toRandom;
		this->toEmpty = another.toEmpty;

		return *this;
	}
};

// プレイヤー
class Player
{
private:
	// 落とすセルの1ループ
	Array<Cell> dropCells1LoopCells;
	// 落とすセルの次のスタック
	Array<Cell> dropCells = Array<Cell>();

	// fieldJust10Timesなどの更新
	// 返り値は消えるものがあるかどうか
	bool updatedField();

public:
	Player(Array<int32> _dropCells1LoopNum = { 0, 1, 1, 1, 1, 1 });
	//Player(const Player& another);
	//Player(Player&& another);

	Player& operator=(const Player& another);

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

	// 負け演出残り時間
	double loseTimer = 0.0;
	// 負け演出時間
	const double loseCoolTime = 2.0;

	// スコア
	int32 score = 0;
	// コンボ回数
	int32 combo = 0;

	// 勝敗
	BattleState state = BattleState::playing;

	// 落とすセルを取得する
	Cell& getDropCell(int32 n);

	// 落とすセルを取得する
	// numを超えても追加しない
	const Cell& getDropCellConst(int32 num) const;

	void update(PlayerKeySet keySet);

	void draw(Point fieldPos, Size cellDrawSize) const;
};

class PlayerData
{
public:
	Player player;
	PlayerKeySet keySet;
	Point fieldPos;

	PlayerData()
	{
		player = Player();
		keySet = PlayerKeySet();
		fieldPos = Point();
	}

	PlayerData(Player _player, PlayerKeySet _keySet, Point _fieldPos)
		: player(_player)
		, keySet(_keySet)
		, fieldPos(_fieldPos)
	{
	}

	PlayerData(const PlayerData& data)
		: player(data.player)
		, keySet(data.keySet)
		, fieldPos(data.fieldPos)
	{
	}

	PlayerData& operator=(const PlayerData& data)
	{
		this->player = data.player;
		this->keySet = data.keySet;
		this->fieldPos = data.fieldPos;
		return *this;
	}
};
