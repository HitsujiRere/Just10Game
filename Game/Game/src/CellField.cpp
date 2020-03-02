
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

	// 初期化する
	Just10Times = Grid<int32>(size);

	// ミノの数字の2次元累積和
	Grid<int32> numCSum(size + Size(1, 1));

	try
	{
		// minoCSumを作成する
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

		// Just10の要素となっている回数の差分を調べる
		// 右端と左端のxを先に決める
		for (auto beginx : step(numCSum.width() - 1))
		{
			for (auto endx = beginx + 1; endx < numCSum.width(); endx++)
			{
				// しゃくとり法で求める
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
						// 差分を加算（いもす法）
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

		// minoJust10TimeDiffGridからminoJust10TimeGridへ累積する
		// x方向へ加算
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
		// y方向へ加算
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
	catch (const std::exception & e)
	{
		Print << Unicode::Widen(e.what());
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

Grid<Point> CellField::getFallTo() const
{
	Grid<Point> FallTo(Field.size());

	for (auto x : step((int32)Field.width()))
	{
		int32 pushY = (int32)Field.height() - (int32)1;
		for (int32 y = (int32)Field.height() - (int32)1; y >= 0; --y)
		{
			if (Field.at(y, x).getNumber() != (int32)CellTypeNumber::Empty)
			{
				FallTo.at(y, x) = Point(x, pushY);
				pushY--;
			}
		}
	}

	return FallTo;
}

void CellField::fallCells(Grid<Point> FallTo)
{
	Grid<Cell> FieldFall(Field.size());

	for (auto p : step(Field.size()))
	{
		FieldFall.at(FallTo.at(p)) = Field.at(p);
	}

	Field = FieldFall;

	updateJust10Times();
}

bool CellField::pushCell(const Cell& cell, int32 x)
{
	if (Field.at(0, x).getNumber() != (int32)CellTypeNumber::Empty)
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
			Field.at(p) = (int32)CellTypeNumber::Empty;
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
	std::function<Color(Point, int32)> colorFunc,
	std::function<Color(Point, int32)> backcolorFunc) const
{
	try
	{
		for (auto p : step(Field.size()))
		{
			Cell cell = Field.at(p);
			Point pos = fieldPos + posFunc(p, cell.getNumber());

			Rect(pos, cellSize).draw(backcolorFunc(p, cell.getNumber()));
			cell.getTexture().resized(cellSize).draw(pos, colorFunc(p, cell.getNumber()));
		}
	}
	catch (const std::exception& e)
	{
		Print << Unicode::Widen(e.what());
	}
}


CellField CellField::RandomField(Size size, int32 maxNumber, bool existsEmpty, bool existsObstruct)
{
	CellField field(size);

	for (auto p : step(field.Field.size()))
	{
		field.Field.at(p) = Cell::RandomCell(maxNumber, existsEmpty, existsObstruct);
	}

	field.updateJust10Times();

	return field;
}