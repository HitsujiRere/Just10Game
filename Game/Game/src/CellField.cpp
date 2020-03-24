
# include "Common.hpp"
# include "CellField.hpp"


CellField::CellField()
	: size(Size())
	, drawsize(Size())
{
	clearField();
}

CellField::CellField(int32 drawWidth, int32 drawHeight)
	: size(drawWidth, drawHeight * 2)
	, drawsize(drawWidth, drawHeight)
{
	clearField();
}

CellField::CellField(Size _drawsize)
	: size(_drawsize.x, _drawsize.y * 2)
	, drawsize(_drawsize)
{
	clearField();
}

void CellField::setCell(Cell& cell, int32 x, int32 y)
{
	field.at(y, x) = cell;

	updateJust10Times();
}

void CellField::setCell(Cell& cell, Point p)
{
	field.at(p) = cell;

	updateJust10Times();
}

void CellField::clearField()
{
	field = Grid<Cell>(size);

	for (auto pos : step(size))
	{
		field.at(pos) = Cell();
	}

	updateJust10Times();
}

void CellField::clearBackField()
{
	for (auto p : step(drawsize, size - drawsize))
	{
		field.at(p) = Cell(CellType::Empty);
	}
}

void CellField::updateJust10Times()
{
	// èâä˙âªÇ∑ÇÈ
	just10Times = Grid<int32>(size);

	// É~ÉmÇÃêîéöÇÃ2éüå≥ó›êœòa
	Grid<int32> numCSum(drawsize + Size(1, 1));

	// minoCSumÇçÏê¨Ç∑ÇÈ
	for (auto p : step(Point(1, 1), numCSum.size() - Point(1, 1)))
	{
		numCSum.at(p) = numCSum.at(p - Point(1, 0)) + numCSum.at(p - Point(0, 1))
			- numCSum.at(p - Point(1, 1))
			+ static_cast<int32>(field.at(p - Point(1, 1)).getType());
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
				int32 sum = numCSum.at(endy, endx) - numCSum.at(beginy, endx)
					- numCSum.at(endy, beginx) + numCSum.at(beginy, beginx);
				//Print << U"sum:" << sum;
				if (sum > 10)
				{
					beginy++;
				}
				else if (sum == 10)
				{
					// ç∑ï™Çâ¡éZÅiÇ¢Ç‡Ç∑ñ@Åj
					just10Times.at(beginy, beginx)++;
					if (endx < just10Times.width())
						just10Times.at(beginy, endx)--;
					if (endy < just10Times.height())
						just10Times.at(endy, beginx)--;
					if (endx < just10Times.width() &&
						endy < just10Times.height())
						just10Times.at(endy, endx)++;

					beginx++;
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
	for (auto p : step(Point(1, 0), drawsize - Point(1, 0)))
	{
		just10Times.at(p) += just10Times.at(p - Point(1, 0));
	}
	// yï˚å¸Ç÷â¡éZ
	for (auto p : step(Point(0, 1), drawsize - Point(0, 1)))
	{
		just10Times.at(p) += just10Times.at(p - Point(0, 1));
	}
}

Grid<Point> CellField::getFallTo() const
{
	Grid<Point> FallTo(size);

	for (auto x : step(size.x))
	{
		int32 pushY = drawsize.y - 1;
		for (auto y : step(size.y - 1, size.y, -1))
			//for (auto y : Range(size.y - 1, 0, -1))
		{
			if (y < drawsize.y && field.at(y, x).getType() != CellType::Empty)
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
	Grid<Point> FloatTo(size);

	for (auto p : step(size))
	{
		FloatTo.at(p) = p.y >= floats.at(p.x) ? Point(p.x, p.y - floats.at(p.x)) : Point(-1, -1);
	}

	return FloatTo;
}

void CellField::moveCells(Grid<Point> moveTo)
{
	Grid<Cell> fieldMoved(size);

	for (auto p : step(size))
	{
		if (moveTo.at(p).x >= 0 && moveTo.at(p).y >= 0)
		{
			fieldMoved.at(moveTo.at(p)) = field.at(p);
		}
	}

	field = fieldMoved;

	updateJust10Times();
}

bool CellField::pushTopCell(const Cell& cell, int32 x)
{
	if (field.at(0, x).getType() != CellType::Empty)
	{
		return false;
	}

	field.at(0, x) = cell;

	updateJust10Times();

	return true;
}

int32 CellField::deleteCells(Grid<int> isDelete)
{
	int32 deleted = 0;
	for (auto p : step(size))
	{
		if (isDelete.at(p) != 0)
		{
			field.at(p) = Cell(CellType::Empty);
			deleted++;
		}
	}

	updateJust10Times();

	return deleted;
}

void CellField::draw(Point fieldPos, Size cellSize, Color backcolor) const
{
	for (auto p : step(size))
	{
		Cell cell = field.at(p);
		Point pos = fieldPos + cellSize * p;

		Rect(pos, cellSize).draw(backcolor);
		cell.getTexture().resized(cellSize).draw(pos);
	}
}

void CellField::draw(Point fieldPos, Size cellSize,
	std::function<Point(Point, CellType)> posFunc,
	std::function<Color(Point, CellType)> colorFunc) const
{
	for (auto p : step(size))
	{
		Cell cell = field.at(p);
		Point pos = fieldPos + posFunc(p, cell.getType());

		cell.getTexture().resized(cellSize).draw(pos, colorFunc(p, cell.getType()));
	}
}

CellField CellField::RandomField(Size drawsize, int32 maxNumber, bool hasEmpty, bool hasObstruct)
{
	CellField cellfield(drawsize);

	for (auto p : step(Size(0, 1), cellfield.getSize() - Size(0, 1)))
	{
		cellfield.field.at(p) = Cell::RandomTypeCell(maxNumber, hasEmpty, hasObstruct);
	}

	cellfield.updateJust10Times();

	return cellfield;
}
