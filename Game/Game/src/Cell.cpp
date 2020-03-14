#include "Cell.hpp"


Array<Texture> Cell::Textures;

void Cell::loadTextures()
{
	const int32 xCnt = 4;
	const int32 yCnt = 4;

	try
	{
		Textures.resize(xCnt * yCnt);

		const Image cellsImage(Resource(U"images/CellImages.png"));
		const int32 width = cellsImage.width() / xCnt;
		const int32 height = cellsImage.width() / yCnt;

		for (auto i : step(xCnt* yCnt))
		{
			const Image cellImage = cellsImage.clipped(width * (i % xCnt), height * (i / xCnt), width, height);

			Textures.at(i) = Texture(cellImage);
		}
	}
	catch (const std::exception & e)
	{
		Print << Unicode::Widen(e.what());
	}
}

Cell::Cell()
{
	Number = (int32)CellTypeNumber::Empty;

	if (Textures.size() == 0)
	{
		loadTextures();
	}
}

Cell::Cell(int32 number) : Number(number)
{
	if (Textures.size() == 0)
	{
		loadTextures();
	}
}

Cell::Cell(CellTypeNumber number) : Number((int32)number)
{
	if (Textures.size() == 0)
	{
		loadTextures();
	}
}

int32 Cell::getNumber() const
{
	return Number;
}

Texture Cell::getTexture() const
{
	if (Number < Textures.size())
	{
		return Textures.at(Number);
	}
	else
	{
		return Texture();
	}
}

Cell Cell::RandomCell(int32 maxNumber, bool existsEmpty, bool existsObstruct)
{
	int32 max = maxNumber;
	if (existsEmpty)	max++;
	if (existsObstruct)	max++;

	int32 number = Random((int32)1, max);

	if (number > maxNumber)
	{
		if (number == maxNumber + 1)
		{
			if (existsEmpty)
				number = (int32)CellTypeNumber::Empty;
			else if (existsObstruct)
				number = (int32)CellTypeNumber::Obstruct;
		}

		if (number == maxNumber + 2)
		{
			if (existsObstruct)
				number = (int32)CellTypeNumber::Obstruct;
		}
	}

	return Cell(number);
}

Array<Texture> Cell::getTextures()
{
	return Textures;
}

Texture Cell::getTexture(int32 num)
{
	return Textures.at(num);
}
