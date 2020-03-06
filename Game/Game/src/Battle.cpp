
# include "Battle.hpp"

Battle::Battle(const InitData& init)
	: IScene(init)
{
	getData().playTime++;
}

void Battle::update()
{
	const double deltaTime = Scene::DeltaTime();

	player.update();

	// 演出処理
	{
		// 負けと表示する演出
		if (player.state == (int32)BattleState::lose)
		{
			loseTimer += deltaTime;

			if (loseTimer > loseWaitTime || KeyZ.down())
			{
				changeScene(State::Title);
			}
		}
	}


	if (KeyEscape.down())
	{
		changeScene(State::Title);
		getData().highScore = Max(getData().highScore, player.m_score);
	}
}

void Battle::draw() const
{
	Rect(0, (int32)(Scene::Height() * 0.7), Scene::Width(), (int32)(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	// スコアの表示
	FontAsset(U"Text")(U"Score:{}"_fmt(player.m_score)).drawAt(Scene::Center().x, 30);

	// Nextセルの描画
	FontAsset(U"Text")(U"Next").drawAt(fieldPos + Vec2(player.fieldSize.x + 2, -0.5) * cellDrawSize);
	player.getDropCellConst(1).getTexture().resized(cellDrawSize * 2).draw(fieldPos + Vec2(player.fieldSize.x + 1, 0) * cellDrawSize);
	for (int32 i = 2; i <= 5; ++i)
	{
		player.getDropCellConst(i).getTexture().resized(cellDrawSize).draw(fieldPos + Vec2(player.fieldSize.x + 1, i) * cellDrawSize);
	}

	// ホールドの表示
	FontAsset(U"Text")(U"Hold").drawAt(fieldPos + Vec2(-2, -0.5) * cellDrawSize);
	player.holdCell.getTexture().resized(cellDrawSize * 2).draw(fieldPos + Point(-3, 0) * cellDrawSize);

	// フィールドの背景
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (player.fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).draw(ColorF(0.2, 0.8, 0.4));
	// フィールドの枠
	Rect(fieldPos - cellDrawSize * Size(0, 1) - Point(5, 5), (player.fieldSize + Size(0, 1)) * cellDrawSize + Point(10, 10)).drawFrame(10, Palette::Forestgreen);

	// Just10の回数の表示
	if (KeyControl.pressed())
	{
		for (auto p : step(player.fieldSize))
		{
			Point pos = fieldPos + cellDrawSize * p + cellDrawSize / 2;
			FontAsset(U"Text")(U"{}"_fmt(player.just10Times.at(p))).draw(pos, Palette::Black);
		}
	}
	// フィールドの描画
	else
	{
		player.field.draw(fieldPos, cellDrawSize,
			[&](Point p, int32) {
				if (player.just10Times.at(p))
					return (p * cellDrawSize);
				else
				{
					const double e = EaseOutCubic(player.fallingTimer / player.fallingCoolTime);
					return (Vec2(p * cellDrawSize).lerp(Vec2(player.fieldMoveTo.at(p) * cellDrawSize), e)).asPoint();
				}
			},
			[&](Point p, int32) {
				if (player.just10Times.at(p))
					return Color(255, (int32)(255 * (1.0 - player.deletingTimer / player.deletingCoolTime)));
				else
					return Color(255, 255);
			},
				[this](Point, int32) {
				return Color(0, 0);
			});
	}

	// 落とすセルの描画
	{
		const Point dropCellPos(fieldPos + Point(cellDrawSize.x * player.dropCellFieldX, -cellDrawSize.y));

		player.getDropCellConst(0).getTexture().resized(cellDrawSize).draw(dropCellPos);
	}

	// 負けという知らせの描画
	if (player.state == (int32)BattleState::lose)
	{
		const double e = EaseOutBounce(loseTimer / loseCoolTime <= 1.0 ? loseTimer / loseCoolTime : 1.0);
		const Vec2 to(fieldPos + player.fieldSize * cellDrawSize / 2.0);
		const Vec2 pos(Vec2(fieldPos.x + player.fieldSize.x * cellDrawSize.x / 2.0, -100).lerp(to, e));
		FontAsset(U"Header")(U"Lose").drawAt(pos, Palette::Black);
		FontAsset(U"Text")(U"タイトルへ戻るまで{:.0f}s"_fmt(loseWaitTime - loseTimer)).drawAt(pos.movedBy(0, 100), Palette::Black);
	}
}
