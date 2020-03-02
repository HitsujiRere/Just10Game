
# include "Battle.hpp"

Battle::Battle(const InitData& init)
	: IScene(init)
{
	getDropCell(10);
}

void Battle::update()
{
	const double deltaTime = Scene::DeltaTime();

	// ���10�ȏ�m�ۂ��Ă����iNext�\���΍�j
	getDropCell(10);

	// ����\
	if (canOperate)
	{
		// �t�B�[���h�������_���ɕύX����
		if (KeyEnter.down())
		{
			field = CellField::RandomField(fieldSize, cellMaxNumber, false, false);
			updatedField();
		}

		// �t�B�[���h�𖳂ɕύX����
		if (KeyBackspace.down())
		{
			field = CellField(fieldSize);
			updatedField();
		}

		// �Z�����z�[���h����
		if (KeyTab.down())
		{
			if (holdCell.getNumber() == (int32)CellTypeNumber::Empty)
			{
				holdCell = getDropCell(0);
				dropCells.remove_at(0);
			}
			else
			{
				Cell tmp = holdCell;
				holdCell = dropCells.at(0);
				dropCells.at(0) = tmp;
			}

			if (debugPrint)	Print << U"Hold...";
		}

		// ���Ƃ��Z����ύX����
		if (KeyUp.down())
		{
			// 1 ~ cellMaxNumber��1������
			int32 nextNumber = getDropCell(0).getNumber() + 1;
			if (nextNumber > cellMaxNumber)
				nextNumber = 1;
			getDropCell(0) = Cell(nextNumber);
		}

		// ���Ƃ��Z�����ړ�����
		if (KeyRight.pressed() || KeyLeft.pressed())
		{
			dropCellTimer += deltaTime;
		}
		if (KeyRight.pressed() && dropCellFieldX < fieldSize.x - 1)
		{
			if (dropCellTimer > dropCellCoolTime)
			{
				dropCellFieldX++;
				dropCellTimer = 0.0;
			}
		}
		if (KeyLeft.pressed() && dropCellFieldX > 0)
		{
			if (dropCellTimer > dropCellCoolTime)
			{
				dropCellFieldX--;
				dropCellTimer = 0.0;
			}
		}
		if (KeyRight.up() || KeyLeft.up())
		{
			dropCellTimer = dropCellCoolTime;
		}

		//�Z���𗎉�������
		if (KeyDown.pressed())
		{
			field.pushCell(getDropCell(0), dropCellFieldX);

			// ���������ֈڍs
			canOperate = false;
			isFallingTime = true;
			fieldMoveTo = field.getFallTo();
			if (debugPrint)	Print << U"Falling...";

			dropCells.remove_at(0);
		}
	}
	// ����s�\�i���o�������s���j
	else
	{
		// �Z���������鉉�o
		if (isDeletingTime)
		{
			deletingTimer += deltaTime;

			if (deletingTimer > deletingCoolTime)
			{
				int32 dc = field.deleteCells(just10Times);
				if (debugPrint)	Print << U"Deleted {} Cells!"_fmt(dc);
				m_score += dc;

				isDeletingTime = false;
				deletingTimer = 0.0;

				// ���������ֈڍs
				isFallingTime = true;
				fieldMoveTo = field.getFallTo();
				if (debugPrint)	Print << U"Falling...";
			}
		}

		// �Z���������鉉�o
		if (isFallingTime)
		{
			fallingTimer += deltaTime;

			if (fallingTimer > fallingCoolTime)
			{
				field.fallCells(fieldMoveTo);
				if (debugPrint)	Print << U"Falled Cells!";

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


	if (KeyEscape.down())
	{
		changeScene(State::Title);
		getData().highScore = Max(getData().highScore, m_score);
	}
}

void Battle::draw() const
{
	Rect(0, Scene::Height() * 0.7, Scene::Width(), Scene::Height() * 0.3)
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	// �X�R�A�̕\��
	FontAsset(U"Text")(U"Score:{}"_fmt(m_score)).drawAt(Scene::Center().x, 30);

	// Next�Z���̕`��
	FontAsset(U"Text")(U"Next").drawAt(fieldPos + Vec2(fieldSize.x + 2, -0.5) * cellDrawSize);
	getDropCellConst(1).getTexture().resized(cellDrawSize * 2).draw(fieldPos + Vec2(fieldSize.x + 1, 0) * cellDrawSize);
	for (int32 i = 2; i <= 5; ++i)
	{
		getDropCellConst(i).getTexture().resized(cellDrawSize).draw(fieldPos + Vec2(fieldSize.x + 1, i) * cellDrawSize);
	}

	// �z�[���h�̕\��
	FontAsset(U"Text")(U"Hold").drawAt(fieldPos + Vec2(-2, -0.5) * cellDrawSize);
	holdCell.getTexture().resized(cellDrawSize * 2).draw(fieldPos + Point(-3, 0) * cellDrawSize);

	// �t�B�[���h�̔w�i
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).draw(ColorF(0.2, 0.8, 0.4));
	// �t�B�[���h�̘g
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).drawFrame(10, Palette::Forestgreen);

	// Just10�̉񐔂̕\��
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
		field.draw(fieldPos, cellDrawSize,
			[this](Point p, int32) {
				if (just10Times.at(p))
					return (p * cellDrawSize);
				else
				{
					double e = EaseOutCubic(fallingTimer / fallingCoolTime);
					return (Vec2(p * cellDrawSize).lerp(Vec2(fieldMoveTo.at(p) * cellDrawSize), e)).asPoint();
				}
			},
			[this](Point p, int32) {
				if (just10Times.at(p))
					return Color(255, (int32)(255 * (1.0 - deletingTimer / deletingCoolTime)));
				else
					return Color(255, 255);
			},
				[this](Point, int32) {
				return Color(0, 0);
			});
	}

	// ���Ƃ��Z���̕`��
	{
		const Point dropCellPos(fieldPos + Point(cellDrawSize.x * dropCellFieldX, -cellDrawSize.y));

		getDropCellConst(0).getTexture().resized(cellDrawSize).draw(dropCellPos);
	}
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

Cell& Battle::getDropCell(int32 num)
{
	// dropCells��ǉ�
	while (num >= dropCells.size())
	{
		Array<Cell> addArray;
		for (auto i : step((int32)dropCellsNumCnt1Loop.size()))
		{
			for (auto j : step(dropCellsNumCnt1Loop.at(i)))
			{
				addArray << Cell(i);
			}
		}
		addArray.shuffle();
		dropCells.append(addArray);
	}

	return dropCells.at(num);
}

const Cell& Battle::getDropCellConst(int32 num) const
{
	return dropCells.at(num);
}
