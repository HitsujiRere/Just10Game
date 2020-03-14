
#pragma once

#ifndef MY_CELL_FIELD_INCLUDED
#	define MY_CELL_FIELD_INCLUDED
#endif

#include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include "Cell.hpp"

// セルを置くフィールド
class CellField
{
private:
	// フィールド
	Grid<Cell> Field;

	// Just10の要素の回数
	Grid<int32> Just10Times;

	// フィールドを初期化する
	void clearField(Size size);

	// Just10Timesを更新する
	void updateJust10Times();
		

public:
	// 0*0のEmptyで初期化
	CellField();

	// width*heightのEmptyで初期化
	CellField(int32 width, int32 height);

	// sizeのEmptyで初期化
	CellField(Size size);

	// Gridを取得する
	const Grid<Cell>& getGrid() const;

	// フィールドのJust10の要素となっている回数のGridを返す
	Grid<int32> getJust10Times() const;

	// フィールドのサイズを取得する
	Size size() const;

	// 落下先のデータを取得する
	Grid<Point> getFallTo() const;

	// 落下先に移動させる
	void moveCells(Grid<Point> FallTo);

	// (x,0)にセルを置く
	// 返り値はできたかどうか
	bool pushTopCell(const Cell& cell, int32 x);

	// x行の下にObstructをn個追加する
	// 返り値はできたかどうか
	bool pushUnderObsructs(int32 x, int32 n);

	// isDeleteの要素が0以外なら削除
	// 返り値は消したセルの数
	int32 deleteCells(Grid<int> isDelete);

	// フィールドを描く
	void draw(Point fieldPos, Size cellSize, Color backcolor = Color(0, 0)) const;

	// フィールドを描く
	void draw(Point fieldPos, Size cellSize, 
		std::function<Point(Point, int32)> posFunc,
		std::function<Color(Point, int32)> colorFunc) const;

	// ランダムな数字のセルのフィールドを返す
	// ただし最上部は空になる
	static CellField RandomField(Size size = Size(0, 0), int32 maxNumber = 9, bool existsEmpty = false, bool existsObstruct = false);
};
