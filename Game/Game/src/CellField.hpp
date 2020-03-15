
#pragma once

#ifndef MY_CELL_FIELD_INCLUDED
#	define MY_CELL_FIELD_INCLUDED
#endif

# include <Siv3D.hpp> // OpenSiv3D v0.4.2
# include "Common.hpp"
# include "Cell.hpp"

// セルを置くフィールド
class CellField
{
private:

	// フィールドのサイズ
	Size size;

	// フィールド
	Grid<Cell> field;

	// Just10の要素の回数
	Grid<int32> just10Times;

	// フィールドを初期化する
	void clearField();

	// Just10Timesを更新する
	void updateJust10Times();

public:

	// サイズが(0,0)のCellTypeがEmptyで初期化する
	CellField();

	// サイズが(width,height)のCellTypeがEmptyで初期化
	CellField(int32 width, int32 height);

	// サイズのCellTypeがEmptyで初期化
	CellField(Size size);

	// フィールドを取得する
	inline const Grid<Cell>& getField() const
	{
		return field;
	}

	// フィールドのJust10の要素となっている回数のGridを返す
	inline const Grid<int32>& getJust10Times() const
	{
		return just10Times;
	}

	// フィールドのサイズを取得する
	inline Size getSize() const
	{
		return size;
	}

	// セルをセットする
	void setCell(Cell& cell, int32 x, int32 y);

	// セルをセットする
	void setCell(Cell& cell, Point p);

	// 落下先のデータを取得する
	Grid<Point> getFallTo() const;

	// 行をfloatsの浮かせた先のデータを取得する
	Grid<Point> getFloatTo(Array<int32> floats) const;

	// 落下先に移動させる
	void moveCells(Grid<Point> FallTo);

	// (x,0)にセルを置く
	// 返り値はできたかどうか
	bool pushTopCell(const Cell& cell, int32 x);

	// isDeleteの要素が0以外なら削除
	// 返り値は消したセルの数
	int32 deleteCells(Grid<int> isDelete);

	// フィールドを描く
	void draw(Point fieldPos, Size cellSize, Color backcolor = Color(0, 0)) const;

	// フィールドを描く
	void draw(Point fieldPos, Size cellSize, 
		std::function<Point(Point, CellType)> posFunc,
		std::function<Color(Point, CellType)> colorFunc) const;

	// ランダムな数字のセルのフィールドを返す
	// ただし最上部は空になる
	static CellField RandomField(Size size = Size(0, 0), int32 maxNumber = 9, bool existsEmpty = false, bool existsObstruct = false);
};
