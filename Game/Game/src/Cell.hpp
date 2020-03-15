
#pragma once

#ifndef MY_CELL_INCLUDED
#	define MY_CELL_INCLUDED
#endif // !MY_CELL_INCLUDED

#include <Siv3D.hpp> // OpenSiv3D v0.4.2

// セルのタイプ
enum class CellType
{
    // 数字が1のセル
    Cell1 = 1,
    // 数字が2のセル
    Cell2,
    // 数字が3のセル
    Cell3,
    // 数字が4のセル
    Cell4,
    // 数字が5のセル
    Cell5,
    // 数字が6のセル
    Cell6,
    // 数字が7のセル
    Cell7,
    // 数字が8のセル
    Cell8,
    // 数字が9のセル
    Cell9,
    // 数字が10のセル
    Cell10,

    // 空のセル
    Empty = 11,
    
    // オジャマセル
    Obstruct = 12,
    
    // 禁止セル
    No = 13,
};

// セル
class Cell
{
private:
    // セルのタイプごとのテクスチャ
    static Array<Texture> typesTexture;

    // テクスチャをロードする
    static void loadTypesTexture();

    // セルのタイプ
    CellType type;

public:
    // Emptyで初期化する
    // 初回はloadTypesTexture()を実行する
    Cell();

    // 指定したタイプで初期化する
    // 初回はloadTypesTexture()を実行する
    Cell(CellType _type);

    // セルのタイプを取得する
    inline CellType getType() const
    {
        return type;
    }
    
    // セルのタイプに対応したテクスチャのコピーを返す
    // Texturesに存在しない場合は空のテクスチャを返す
    const Texture getTexture() const
    {
        int32 number = static_cast<int32>(type);

        if (number < typesTexture.size())
        {
            return typesTexture.at(number);
        }
        else
        {
            return Texture();
        }
    }

    // ランダムなタイプのセルを返す
    static Cell& RandomTypeCell(int32 maxTypeNumber = 9, bool hasEmpty = false, bool hasObstruct = false);

    // セルのタイプごとのテクスチャを返す
    static inline const Array<Texture>& getTypesTexture()
    {
        return typesTexture;
    }

    // 指定したタイプのテクスチャを返す
    static inline const Texture getTexture(CellType type)
    {
        int32 number = static_cast<int32>(type);

        if (number < typesTexture.size())
        {
            return typesTexture.at(number);
        }
        else
        {
            return Texture();
        }
    }

    inline operator String() const
    {
        return Format(static_cast<int32>(type));
    }
};
