
# pragma once

# include <Siv3D.hpp>

// �L�����N�^�[
class Character
{
private:

	// �L�����N�^�[����
	static Array<Character> characters;

	// characters���w��p�X���烍�[�h����
	static void loadCharactersFromPath(String path, bool IsResourcePath = false);

public:
	int32 number;
	String name;
	Texture texture;
	String desc;

	Character();

	// �L�����N�^�[������Ԃ�
	inline static const Array<Character>& getCharacters()
	{
		if (characters.isEmpty())
		{
			loadCharacters();
		}

		return characters;
	}

	// characters�����[�h����
	static void loadCharacters();
};
