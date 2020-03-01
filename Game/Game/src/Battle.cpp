
# include "Battle.hpp"

Battle::Battle(const InitData& init)
	: IScene(init)
{
}

void Battle::update()
{
	// ����\
	if (canOperate)
	{
		// �t�B�[���h�������_���ɕύX����
		if (KeyEnter.down())
		{
			field = CellField::GetRandomField(fieldSize, cellMaxNumber, false, false);
			updatedField();
		}
		// �t�B�[���h�𖳂ɕύX����
		if (KeyBackspace.down())
		{
			field = CellField(fieldSize);
			updatedField();
		}

		// ���Ƃ��Z����ύX����
		if (KeyUp.down())
		{
			// 1 ~ cellMaxNumber��1������
			int32 nextNumber = dropCell.getNumber() + 1;
			if (nextNumber > cellMaxNumber)
				nextNumber = 1;
			dropCell = Cell(nextNumber);
		}
		// ���Ƃ��Z�����ړ�����
		if (KeyRight.down() && dropCellFieldX < fieldSize.x - 1)
		{
			dropCellFieldX++;
		}
		if (KeyLeft.down() && dropCellFieldX > 0)
		{
			dropCellFieldX--;
		}
		//�Z���𗎉�������
		if (KeyDown.down())
		{
			field.pushCell(dropCell, dropCellFieldX);

			// ���������ֈڍs
			canOperate = false;
			isFallingTime = true;
			fieldMoveTo = field.getFallTo();
			Print << U"Falling...";

			//dropCell = Cell::getRandomCell(cellMaxNumber);
		}
	}
	// ����s�\�i���o�������s���j
	else
	{
		const double deltaTime = Scene::DeltaTime();

		// �Z���������鉉�o
		if (isDeletingTime)
		{
			deletingTimer += deltaTime;

			if (deletingTimer > deletingCoolTime)
			{
				int32 dc = field.deleteCells(just10Times);
				Print << U"Deleted {} Cells!"_fmt(dc);

				isDeletingTime = false;
				deletingTimer = 0.0;

				// ���������ֈڍs
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

		// �Z���������鉉�o
		if (isFallingTime)
		{
			fallingTimer += deltaTime;

			if (fallingTimer > fallingCoolTime)
			{
				field.fallCells(fieldMoveTo);
				Print << U"Falled Cells!";

				isFallingTime = false;
				fallingTimer = 0.0;

				// ��������̂�����Ȃ�A�����鉉�o��
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
	// �t�B�[���h�̕`��
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

	// ���Ƃ��Z���̕\��
	{
		const Point dropCellPos(fieldPos + Point(cellDrawSize.x * dropCellFieldX, -cellDrawSize.y));

		dropCell.getTexture().resized(cellDrawSize).draw(dropCellPos);
	}

	//Print << field.getGrid();
}


bool Battle::updatedField()
{
	just10Times = field.getJust10Times();

	// Just10�ŏ�������̂�����Ȃ�A�����鎞�Ԃɂ���
	if (just10Times.any([](int32 n) { return n != 0; }))
	{
		canOperate = false;
		isDeletingTime = true;
		if (debugPrint)	Print << U"Deleting...";

		return true;
	}

	return false;
}
