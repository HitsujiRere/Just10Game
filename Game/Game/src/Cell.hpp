
#pragma once

#ifndef MY_CELL_INCLUDED
#	define MY_CELL_INCLUDED
#endif // !MY_CELL_INCLUDED

#include <Siv3D.hpp> // OpenSiv3D v0.4.2

// セルのタイプごとの数字
enum class CellTypeNumber
{
	Cell1 = 1,
	Cell2,
	Cell3,
	Cell4,
	Cell5,
	Cell6,
	Cell7,
	Cell8,
	Cell9,
	Cell10,
	Empty = 11,
	Obstruct = 12,
};


// セル（単位ブロック）
class Cell
{
private:
	// セルの数字ごとのテクスチャ
	static Array<Texture> Textures;

	// セルのテクスチャをロードする
	static void loadTextures();

	// セルの数字
	int32 Number;

public:
	// Emptyで初期化
	// 初回はLoadTextures()を実行する
	Cell();

	// numberで初期化
	// 初回はLoadTextures()を実行する
	Cell(int number);

	// 数字を取得する
	int32 getNumber() const;

	// 現在のセルのテクスチャのコピーを返す
	// Texturesに存在しない場合は空のテクスチャを返す
	Texture getTexture() const;

	// ランダムな数字のセルを返す
	static Cell getRandomCell(int32 maxNumber = 9, bool existsEmpty = false, bool existsObstruct = false);
};
