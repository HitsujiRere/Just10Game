
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

	player.draw(fieldPos, cellSize);

	// 負けという知らせの描画
	if (player.state == (int32)BattleState::lose)
	{
		const double e = EaseOutBounce(loseTimer / loseCoolTime <= 1.0 ? loseTimer / loseCoolTime : 1.0);
		const Vec2 to(fieldPos + player.fieldSize * cellSize / 2.0);
		const Vec2 pos(Vec2(fieldPos.x + player.fieldSize.x * cellSize.x / 2.0, -100).lerp(to, e));
		FontAsset(U"Header")(U"Lose").drawAt(pos, Palette::Black);
		FontAsset(U"Text")(U"タイトルへ戻るまで{:.0f}s"_fmt(loseWaitTime - loseTimer)).drawAt(pos.movedBy(0, 100), Palette::Black);
	}
}
