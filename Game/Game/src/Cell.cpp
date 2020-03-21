#include "Cell.hpp"


Array<Texture> Cell::typesTexture;

void Cell::loadTypesTexture()
{
	const int32 xCnt = 4;
	const int32 yCnt = 4;

	try
	{
		typesTexture.resize(xCnt * yCnt);

		const Image cellsImage(Resource(U"images/CellImages.png"));
		const int32 width = cellsImage.width() / xCnt;
		const int32 height = cellsImage.width() / yCnt;

		for (auto i : step(xCnt* yCnt))
		{
			const Image cellImage = cellsImage.clipped(width * (i % xCnt), height * (i / xCnt), width, height);

			typesTexture.at(i) = Texture(cellImage);
		}
	}
	catch (const Error & e)
	{
		Print << e.what();
	}
}

Cell::Cell()
{
	type = CellType::Empty;

	if (typesTexture.isEmpty())
	{
		loadTypesTexture();
	}
}

Cell::Cell(CellType _type) : type(_type)
{
	if (typesTexture.size() == 0)
	{
		loadTypesTexture();
	}
}

Cell Cell::RandomTypeCell(int32 maxTypeNumber, bool hasEmpty, bool hasObstruct)
{
	Array<CellType> all;
	for (int32 i = 0; i < maxTypeNumber; ++i)
	{
		all.push_back(static_cast<CellType>(i));
	}

	if (hasEmpty)
	{
		all.push_back(CellType::Empty);
	}
	if (hasObstruct)
	{
		all.push_back(CellType::Obstruct);
	}

	Cell cell(all.choice());

	return cell;
}
