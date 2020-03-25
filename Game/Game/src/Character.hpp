
# pragma once

# include <Siv3D.hpp>

// キャラクター
class Character
{
private:

	// キャラクターたち
	static Array<Character> characters;

	// charactersを指定パスからロードする
	static void loadCharactersFromPath(String path, bool IsResourcePath = false);

public:

	// キャラクター識別子
	String identity;
	// キャラクター番号
	int32 number;
	// 名前
	String name;
	// 説明
	String desc;
	// テクスチャー
	Texture texture;
	// 落とすセルの1ループ
	Array<int32> dropCells1LoopNum;
	// 与えるダメージ割合
	double atkRate;
	// 与えられるダメージ割合
	double defRate;

	Character();

	// キャラクターたちを返す
	inline static const Array<Character>& getCharacters()
	{
		if (characters.isEmpty())
		{
			loadCharacters();
		}

		return characters;
	}

	// charactersをロードする
	static void loadCharacters();
};
