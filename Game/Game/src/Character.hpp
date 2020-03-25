
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

	// �L�����N�^�[���ʎq
	String identity;
	// �L�����N�^�[�ԍ�
	int32 number;
	// ���O
	String name;
	// ����
	String desc;
	// �e�N�X�`���[
	Texture texture;
	// ���Ƃ��Z����1���[�v
	Array<int32> dropCells1LoopNum;
	// �^����_���[�W����
	double atkRate;
	// �^������_���[�W����
	double defRate;

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
