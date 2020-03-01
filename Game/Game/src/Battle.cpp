
# include "Battle.hpp"

Battle::Battle(const InitData& init)
	: IScene(init)
{
}

void Battle::update()
{
	// 操作可能
	if (canOperate)
	{
		// フィールドをランダムに変更する
		if (KeyEnter.down())
		{
			field = CellField::GetRandomField(fieldSize, cellMaxNumber, false, false);
			updatedField();
		}
		// フィールドを無に変更する
		if (KeyBackspace.down())
		{
			field = CellField(fieldSize);
			updatedField();
		}

		// 落とすセルを変更する
		if (KeyUp.down())
		{
			// 1 ~ cellMaxNumberで1周する
			int32 nextNumber = dropCell.getNumber() + 1;
			if (nextNumber > cellMaxNumber)
				nextNumber = 1;
			dropCell = Cell(nextNumber);
		}
		// 落とすセルを移動する
		if (KeyRight.down() && dropCellFieldX < fieldSize.x - 1)
		{
			dropCellFieldX++;
		}
		if (KeyLeft.down() && dropCellFieldX > 0)
		{
			dropCellFieldX--;
		}
		//セルを落下させる
		if (KeyDown.down())
		{
			field.pushCell(dropCell, dropCellFieldX);

			// 落下処理へ移行
			canOperate = false;
			isFallingTime = true;
			fieldMoveTo = field.getFallTo();
			Print << U"Falling...";

			//dropCell = Cell::getRandomCell(cellMaxNumber);
		}
	}
	// 操作不可能（演出処理実行中）
	else
	{
		const double deltaTime = Scene::DeltaTime();

		// セルが消える演出
		if (isDeletingTime)
		{
			deletingTimer += deltaTime;

			if (deletingTimer > deletingCoolTime)
			{
				int32 dc = field.deleteCells(just10Times);
				Print << U"Deleted {} Cells!"_fmt(dc);

				isDeletingTime = false;
				deletingTimer = 0.0;

				// 落下処理へ移行
				isFallingTime = true;
				fieldMoveTo = field.getFallTo();
				Print << U"Falling...";

				//field.fallCells(fieldMoveTo);
				//fieldJust10Time = field.getJust10Times();

				/*if (fieldJust10Time.any([](int32 n) { return n != 0; }))
				{
					deletingJust10Timer = 0.0;
					Print << U"Deleting...";
				}*/
			}
		}

		// セルが落ちる演出
		if (isFallingTime)
		{
			fallingTimer += deltaTime;

			if (fallingTimer > fallingCoolTime)
			{
				field.fallCells(fieldMoveTo);
				Print << U"Falled Cells!";

				isFallingTime = false;
				fallingTimer = 0.0;

				// 消えるものがあるなら、消える演出へ
				if (!updatedField())
				{
					canOperate = true;
				}
			}
		}
	}

	/*if (false)
	{
		changeScene(State::Title);
		getData().highScore = Max(getData().highScore, m_score);
	}*/
}

void Battle::draw() const
{
	if (KeyControl.pressed())
	{
		for (auto p : step(fieldSize))
		{
			Point pos = fieldPos + cellDrawSize * p + cellDrawSize / 2;
			FontAsset(U"Text")(U"{}"_fmt(just10Times.at(p))).draw(pos, Palette::Black);
		}
	}
	// フィールドの描画
	else
	{
		/*field.draw(fieldPos, cellSize,
			[&just10Times, &fieldMoveTo, deleteJust10Timer, cellSize](Point p, int32 n) {
				if (just10Times.at(p))
					return (p * cellSize);
				else
					return ((p + Point(0, 1) * deleteJust10Timer) * cellSize).asPoint();
				//return (((fieldMoveTo.at(p) - p) * deleteJust10Timer + p) * cellSize).asPoint();
			},
			[&just10Times, deletingTimer](Point p, int32 n) {
				if (just10Times.at(p))
					return Color(255, 255 * (1.0 - deleteJust10Timer / 1.0));
				else
					return Color(255, 255);
			});*/

		field.draw(fieldPos, cellDrawSize);
	}

	// 落とすセルの表示
	{
		const Point dropCellPos(fieldPos + Point(cellDrawSize.x * dropCellFieldX, -cellDrawSize.y));

		dropCell.getTexture().resized(cellDrawSize).draw(dropCellPos);
	}

	//Print << field.getGrid();
}


bool Battle::updatedField()
{
	just10Times = field.getJust10Times();

	// Just10で消えるものがあるなら、消える時間にする
	if (just10Times.any([](int32 n) { return n != 0; }))
	{
		canOperate = false;
		isDeletingTime = true;
		if (debugPrint)	Print << U"Deleting...";

		return true;
	}

	return false;
}
