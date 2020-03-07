
# include "Battle.hpp"

Battle::Battle(const InitData& init)
	: IScene(init)
{
	getData().playTime++;

	{
		auto player = Player();
		const KeyGroup moveL = (KeyA | Key());
		const KeyGroup moveR = (KeyD | Key());
		const KeyGroup drop = (KeyS | Key());
		const KeyGroup hold = (KeyW | Key());
		playerDatas << PlayerData(player, PlayerKeySet(moveL, moveR, drop, hold), Point(cellSize * Size(4, 2)));
	}
	{
		auto player = Player();
		player.fieldColor = Color(231, 184, 194);
		const KeyGroup moveL = (KeyLeft | Key());
		const KeyGroup moveR = (KeyRight | Key());
		const KeyGroup drop = (KeyDown | Key());
		const KeyGroup hold = (KeyUp | Key());
		playerDatas << PlayerData(player, PlayerKeySet(moveL, moveR, drop, hold), Point(cellSize * Size(4, 2)).movedBy(Scene::Center().x, 0));
	}
}

void Battle::update()
{
	const double deltaTime = Scene::DeltaTime();

	for (auto& playerData : playerDatas)
	{
		{
			playerData.player.update(playerData.keySet);
		}

		// 演出処理
		{
			// 負けと表示する演出
			if (playerData.player.state == (int32)BattleState::lose)
			{
				if (playerData.player.loseTimer > loseWaitTime || KeyZ.down())
				{
					changeScene(State::Title);
				}
			}
		}
	}

	if (backKeys.down())
	{
		int32 hightScore = getData().highScore;
		for (auto& playerData : playerDatas)
		{
			hightScore = Max(hightScore, playerData.player.m_score);
		}
		getData().highScore = hightScore;
		changeScene(State::Title);
	}
}

void Battle::draw() const
{
	Rect(0, (int32)(Scene::Height() * 0.7), Scene::Width(), (int32)(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	for (auto& playerData : playerDatas)
	{
		playerData.player.draw(playerData.fieldPos, cellSize);

		// 負けという知らせの描画
		if (playerData.player.state == (int32)BattleState::lose)
		{
			//const double e = EaseOutBounce(Min(player.first.loseTimer / player.first.loseCoolTime, 1.0));
			//const Vec2 to(fieldPos + player.first.fieldSize * cellSize / 2.0);
			//const Vec2 pos(Vec2(fieldPos.x + player.first.fieldSize.x * cellSize.x / 2.0, -100).lerp(to, e));
			//FontAsset(U"Text")(U"タイトルへ戻るまで{:.0f}s"_fmt(Max(loseWaitTime - player.first.loseTimer, 0.0)))
				//.drawAt(pos.movedBy(0, 100), ColorF(0.0));
			FontAsset(U"Text")(U"タイトルへ戻るまで{:.0f}s"_fmt(Max(loseWaitTime - playerData.player.loseTimer, 0.0)))
				.drawAt(Scene::Center(), ColorF(0.0));
		}
	}
}
