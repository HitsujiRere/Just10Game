# include "Character.hpp"

Array<Character> Character::characters;

Character::Character()
	: identity(U"Default")
	, number(0)
	, name(U"�f�t�H���g����")
	, desc(U"�f�B�t�H���g�ł��B")
	, texture(Texture())
	, dropCells1LoopNum({0, 1, 1, 1, 1, 1})
	, atkRate(1.0)
	, defRate(1.0)
{
}

void Character::loadCharactersFromPath(String path, bool IsResourcePath)
{
	const INIData ini(IsResourcePath ? Resource(path) : path);

	if (!ini)
	{
		return;
	}

	for (const auto& section : ini.sections())
	{
		try
		{
			Character character;

			character.number = Parse<int32>(ini[U"{}.number"_fmt(section.section)]);

			if (character.number < 0)	continue;

			character.identity = section.section;
			character.name = ini[U"{}.name"_fmt(section.section)];
			character.desc = ini[U"{}.desc"_fmt(section.section)].replaced(U"\\n", U"\n");
			auto isResourceImgTxt = ini[U"{}.isResourceImage"_fmt(section.section)];
			auto isResourceImg = isResourceImgTxt.isEmpty() ? false : Parse<bool>(isResourceImgTxt);
			auto imgPath = ini[U"{}.image"_fmt(section.section)];
			character.texture = Texture(isResourceImg ? Resource(imgPath) : imgPath);
			character.dropCells1LoopNum = ini[U"{}.1loop"_fmt(section.section)].split(U',').map(Parse<int32>);
			character.atkRate = Parse<double>(ini[U"{}.atk"_fmt(section.section)]);
			character.defRate = Parse<double>(ini[U"{}.def"_fmt(section.section)]);

			if (character.number >= characters.size())
			{
				characters.resize(character.number + 1);
			}
			characters.at(character.number) = character;
		}
		catch (const Error & e)
		{
			Print << e.what();
		}
	}
}

void Character::loadCharacters()
{
	loadCharactersFromPath(U"Text/charDataResource.ini", true);
	loadCharactersFromPath(U"data/charData.ini");

	// ��̏ꍇ�́A�f�B�t�H���g�����ǉ�����
	if (characters.isEmpty())
	{
		characters << Character();
	}
}