
# pragma once

# include <Siv3D.hpp>

// �L�����N�^�[
class Character
{
private:

	// �L�����N�^�[����
	static Array<Character> characters;

	// characters���w��p�X���烍�[�h����
	static void loadCharacters(String path, bool IsResourcePath = false);

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
			loadCharacters(U"charDataResource.ini", true);
			//loadCharacters(U"charData.ini");

			// ��̏ꍇ�́A�f�B�t�H���g�����ǉ�����
			if (characters.isEmpty())
			{
				characters << Character();
			}
		}

		return characters;
	}
};
