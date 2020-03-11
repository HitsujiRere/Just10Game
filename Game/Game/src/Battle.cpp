
# include "Battle.hpp"

BattleType Battle::battleType = BattleType::By1;

Battle::Battle(const InitData& init)
	: IScene(init)
{
	getData().playTime++;

	if (battleType == BattleType::By1)
	{
		auto player = Player();
		const KeyGroup moveL = (KeyA | KeyLeft);
		const KeyGroup moveR = (KeyD | KeyRight);
		const KeyGroup drop = (KeyS | KeyDown);
		const KeyGroup hold = (KeyW | KeyUp);
		playerDatas << PlayerData(player, PlayerKeySet(moveL, moveR, drop, hold), Point(cellSize * Size(4, 3)));
	}
	
	if (battleType == BattleType::By2)
	{
		{
			auto player = Player();
			const KeyGroup moveL = (KeyA | Key());
			const KeyGroup moveR = (KeyD | Key());
			const KeyGroup drop = (KeyS | Key());
			const KeyGroup hold = (KeyW | Key());
			playerDatas << PlayerData(player, PlayerKeySet(moveL, moveR, drop, hold), Point(cellSize * Size(4, 3)));
		}
		{
			auto player = Player();
			player.fieldColor = Color(231, 184, 194);
			const KeyGroup moveL = (KeyLeft | Key());
			const KeyGroup moveR = (KeyRight | Key());
			const KeyGroup drop = (KeyDown | Key());
			const KeyGroup hold = (KeyUp | Key());
			playerDatas << PlayerData(player, PlayerKeySet(moveL, moveR, drop, hold), Point(cellSize * Size(4, 3)).movedBy(Scene::Center().x, 0));
		}
	}
}

void Battle::update()
{
	// デバッグ用死亡
	if (Key4.down())
	{
		playerDatas.at(0).player.state = BattleState::lose;
	}

	const double deltaTime = Scene::DeltaTime();

	for (auto& playerData : playerDatas)
	{
		playerData.player.update(playerData.keySet);

		// 負けと表示する演出
		if (playerData.player.state == BattleState::lose)
		{
			isFinished = true;
			backTimer += deltaTime;

			if (backTimer > backTime || backKeys.down())
			{
				changeScene(State::Title);
			}
		}
	}

	if (backKeys.down())
	{
		int32 hightScore = getData().highScore;
		for (auto& playerData : playerDatas)
		{
			hightScore = Max(hightScore, playerData.player.score);
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

		// スコア
		FontAsset(U"Text")(U"Score:{}"_fmt(playerData.player.score))
			.drawAt(playerData.fieldPos.movedBy(playerData.player.fieldSize.x * cellSize.x / 2, playerData.player.fieldSize.y * cellSize.y + 30), ColorF(0.25));
		// コンボ
		FontAsset(U"Text")(U"Combo:{}"_fmt(playerData.player.combo))
			.drawAt(playerData.fieldPos.movedBy(playerData.player.fieldSize.x * cellSize.x / 2, playerData.player.fieldSize.y * cellSize.y + 75), ColorF(0.25));

		// 負けの知らせ
		if (playerData.player.state != BattleState::playing)
		{
			const double e = EaseOutBounce(playerData.player.loseTimer / playerData.player.loseCoolTime <= 1.0 ? playerData.player.loseTimer / playerData.player.loseCoolTime : 1.0);
			const Vec2 to(playerData.fieldPos + playerData.player.fieldSize * cellSize / 2.0);
			const Vec2 pos(Vec2(playerData.fieldPos.x + playerData.player.fieldSize.x * cellSize.x / 2.0, -100).lerp(to, e));
			String text = U"";
			if (playerData.player.state == BattleState::win)	text = U"Win";
			if (playerData.player.state == BattleState::tie)	text = U"Tie";
			if (playerData.player.state == BattleState::lose)	text = U"Lose";
			FontAsset(U"Header")(text).drawAt(pos, Palette::Black);
		}
	}

	// タイトルに戻るカウントダウン
	if (isFinished)
	{
		FontAsset(U"Text")(U"タイトルへ戻るまで{:.0f}s"_fmt(Max(backTime - backTimer, 0.0)))
			.drawAt(Scene::Center(), ColorF(0.0));
	}
}
