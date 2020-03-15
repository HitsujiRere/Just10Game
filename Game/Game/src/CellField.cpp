
#include "CellField.hpp"


void CellField::clearField(Size size)
{
	Field = Grid<Cell>(size);

	for (auto pos : step(size))
	{
		Field.at(pos) = Cell();
	}

	updateJust10Times();
}

void CellField::updateJust10Times()
{
	const auto size = Field.size();

	// èâä˙âªÇ∑ÇÈ
	Just10Times = Grid<int32>(size);

	// É~ÉmÇÃêîéöÇÃ2éüå≥ó›êœòa
	Grid<int32> numCSum(size + Size(1, 1));

	// minoCSumÇçÏê¨Ç∑ÇÈ
	for (auto p : step(numCSum.size()))
	{
		int32 n = 0;
		if (p.x > 0)
		{
			n += numCSum[p - Point(1, 0)];
		}
		if (p.y > 0)
		{
			n += numCSum[p - Point(0, 1)];
		}
		if (p.x > 0 && p.y > 0)
		{
			n -= numCSum[p - Point(1, 1)];
			n += Field[p - Point(1, 1)].getNumber();
		}

		numCSum[p] = n;
	}

	// Just10ÇÃóvëfÇ∆Ç»Ç¡ÇƒÇ¢ÇÈâÒêîÇÃç∑ï™Çí≤Ç◊ÇÈ
	// âEí[Ç∆ç∂í[ÇÃxÇêÊÇ…åàÇﬂÇÈ
	for (auto beginx : step(numCSum.width() - 1))
	{
		for (auto endx : step(beginx + 1, numCSum.width() - beginx - 1))
		{
			// ÇµÇ·Ç≠Ç∆ÇËñ@Ç≈ãÅÇﬂÇÈ
			int32 beginy = 0;
			int32 endy = 1;
			while (beginy < numCSum.height() - 1 &&
				endy < numCSum.height())
			{
				//Print << U"( {} ~ {} , {} ~ {} )"_fmt(beginx, endx, beginy, endy);
				int32 sum = numCSum.at(endy, endx)
					- numCSum.at(beginy, endx)
					- numCSum.at(endy, beginx)
					+ numCSum.at(beginy, beginx);
				//Print << U"sum:" << sum;
				if (sum > 10)
				{
					beginy++;
				}
				else if (sum == 10)
				{
					// ç∑ï™Çâ¡éZÅiÇ¢Ç‡Ç∑ñ@Åj
					Just10Times.at(beginy, beginx)++;
					if (endx < Just10Times.width())
						Just10Times.at(beginy, endx)--;
					if (endy < Just10Times.height())
						Just10Times.at(endy, beginx)--;
					if (endx < Just10Times.width() &&
						endy < Just10Times.height())
						Just10Times.at(endy, endx)++;

					endy++;
				}
				else // sum < 0
				{
					endy++;
				}
			}
		}
	}

	// ó›êœÇ∑ÇÈ
	// xï˚å¸Ç÷â¡éZ
	for (auto p : step(size))
	{
		int32 n = 0;
		if (p.x == 0)
		{
			n += Just10Times[p];
		}
		else // p.x > 0
		{
			n += Just10Times[p - Point(1, 0)];
			n += Just10Times[p];
		}

		Just10Times[p] = n;
	}
	// yï˚å¸Ç÷â¡éZ
	for (auto p : step(size))
	{
		int32 n = 0;
		if (p.y == 0)
		{
			n += Just10Times[p];
		}
		else // p.y > 0
		{
			n += Just10Times[p - Point(0, 1)];
			n += Just10Times[p];
		}

		Just10Times[p] = n;
	}
}

CellField::CellField()
{
	clearField(Size());
}

CellField::CellField(int32 width, int32 height)
{
	clearField(Size(width, height));
}

CellField::CellField(Size size)
{
	clearField(size);
}

const Grid<Cell>& CellField::getGrid() const
{
	return Field;
}

Grid<int32> CellField::getJust10Times() const
{
	return Just10Times;
}

Size CellField::size() const
{
	return Field.size();
}

void CellField::setCell(Cell& cell, int32 x, int32 y)
{
	Field.at(y, x) = cell;
	updateJust10Times();
}

void CellField::setCell(Cell& cell, Point p)
{
	Field.at(p) = cell;
	updateJust10Times();
}

Grid<Point> CellField::getFallTo() const
{
	Grid<Point> FallTo(Field.size());

	for (auto x : step((int32)Field.width()))
	{
		int32 pushY = (int32)Field.height() - (int32)1;
		for (auto y : step(Field.height() - 1, Field.height(), -1))
		{
			if (Field.at(y, x).getNumber() != (int32)CellType::Empty)
			{
				FallTo.at(y, x) = Point(x, pushY);
				pushY--;
			}
			else
			{
				FallTo.at(y, x) = Point(-1, -1);
			}
		}
	}

	return FallTo;
}

Grid<Point> CellField::getFloatTo(Array<int32> floats) const
{
	if (Setting::debugPrint)	Print << U"begin CellField::getFloatTo()";

	Size size = this->size();
	Grid<Point> FloatTo(size);

	if (Setting::debugPrint)	Print << U"floats";
	if (Setting::debugPrint)	Print << floats;

	try
	{
		for (auto x : step(size.x))
		{
			for (auto y : step(size.y))
			{
				FloatTo.at(y, x) = y >= floats[x] ? Point(x, y - floats[x]) : Point(-1, -1);
			}
		}
	}
	catch (std::exception & e)
	{
		Print << Unicode::Widen(e.what()) << U" in getFloatTo()";
	}

	if (Setting::debugPrint)	Print << U"end CellField::getFloatTo()";

	if (Setting::debugPrint)	Print << U"FloatTo";
	if (Setting::debugPrint)	Print << FloatTo;

	return FloatTo;
}

void CellField::moveCells(Grid<Point> moveTo)
{
	if (Setting::debugPrint || true)	Print << U"begin CellField::moveCells()";
	Grid<Cell> fieldMoved(Field.size());

	for (auto p : step(Field.size()))
	{
		try
		{
			if (moveTo.at(p).x >= 0 && moveTo.at(p).y >= 0)
			{
				fieldMoved.at(moveTo.at(p)) = Field.at(p);
			}
		}
		catch (std::exception& e)
		{
			Print << Unicode::Widen(e.what()) << U"in moveCells()";
		}
	}

	Field = fieldMoved;

	if (Setting::debugPrint || true)	Print << U"end CellField::moveCells()";
	updateJust10Times();
}

bool CellField::pushTopCell(const Cell& cell, int32 x)
{
	if (Field.at(0, x).getNumber() != (int32)CellType::Empty)
	{
		return false;
	}

	Field.at(0, x) = cell;

	updateJust10Times();

	return true;
}

int32 CellField::deleteCells(Grid<int> isDelete)
{
	int32 deleted = 0;
	for (auto p : step(Field.size()))
	{
		if (isDelete.at(p) != 0)
		{
			Field.at(p) = (int32)CellType::Empty;
			deleted++;
		}
	}

	updateJust10Times();

	return deleted;
}

void CellField::draw(Point fieldPos, Size cellSize, Color backcolor) const
{
	for (auto p : step(Field.size()))
	{
		Cell cell = Field.at(p);
		Point pos = fieldPos + cellSize * p;

		Rect(pos, cellSize).draw(backcolor);
		cell.getTexture().resized(cellSize).draw(pos);
	}
}

void CellField::draw(Point fieldPos, Size cellSize,
	std::function<Point(Point, int32)> posFunc,
	std::function<Color(Point, int32)> colorFunc) const
{
	for (auto p : step(Field.size()))
	{
		Cell cell = Field.at(p);
		Point pos = fieldPos + posFunc(p, cell.getNumber());

		cell.getTexture().resized(cellSize).draw(pos, colorFunc(p, cell.getNumber()));
	}
}

CellField CellField::RandomField(Size size, int32 maxNumber, bool existsEmpty, bool existsObstruct)
{
	CellField field(size);

	for (auto p : step(Size(0, 1), field.Field.size() - Size(0, 1)))
	{
		field.Field.at(p) = Cell::RandomCell(maxNumber, existsEmpty, existsObstruct);
	}

	field.updateJust10Times();

	return field;
}
