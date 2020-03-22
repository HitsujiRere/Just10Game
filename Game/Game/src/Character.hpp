
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
			loadCharacters();
		}

		return characters;
	}

	// charactersをロードする
	static void loadCharacters();
};
