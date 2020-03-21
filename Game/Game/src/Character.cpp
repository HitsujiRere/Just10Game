# include "Character.hpp"

Array<Character> Character::characters;

Character::Character()
	: number(0)
	, name(U"デフォルトくん")
	, desc(U"ディフォルトです。")
	, texture(Texture())
{
}

void Character::loadCharacters(String path, bool IsResourcePath)
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

			character.name = ini[U"{}.name"_fmt(section.section)];
			character.desc = ini[U"{}.desc "_fmt(section.section)];
			auto isResourceImgTxt = ini[U"{}.isResourceImage"_fmt(section.section)];
			auto isResourceImg = isResourceImgTxt.isEmpty() ? false : Parse<bool>(isResourceImgTxt);
			auto imgPath = ini[U"{}.image"_fmt(section.section)];
			character.texture = Texture(isResourceImg ? Resource(imgPath) : imgPath);

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
