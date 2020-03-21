
# pragma once

# include <Siv3D.hpp>

// キャラクター
class Character
{
private:

	// キャラクターたち
	static Array<Character> characters;

	// charactersを指定パスからロードする
	static void loadCharacters(String path, bool IsResourcePath = false);

public:
	int32 number;
	String name;
	Texture texture;
	String desc;

	Character();

	// キャラクターたちを返す
	inline static const Array<Character>& getCharacters()
	{
		if (characters.isEmpty())
		{
			loadCharacters(U"charDataResource.ini", true);
			//loadCharacters(U"charData.ini");

			// 空の場合は、ディフォルトくんを追加する
			if (characters.isEmpty())
			{
				characters << Character();
			}
		}

		return characters;
	}
};
