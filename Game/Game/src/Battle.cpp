
# include "Battle.hpp"

PlayerCount Battle::playerCnt = PlayerCount::By1;

Battle::Battle(const InitData& init)
	: IScene(init)
{
	getData().playTime++;

	if (playerCnt == PlayerCount::By1)
	{
		auto player = Player();
		const KeyGroup moveL = (KeyA | KeyLeft);
		const KeyGroup moveR = (KeyD | KeyRight);
		const KeyGroup drop = (KeyS | KeyDown);
		const KeyGroup hold = (KeyW | KeyUp);
		playerDatas << PlayerData(player, PlayerKeySet(moveL, moveR, drop, hold), Point(cellSize * Size(4, 3)));
	}

	if (playerCnt == PlayerCount::By2)
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
	if (Setting::debugPrint)	Print << U"in Battle::update()";

	// デバッグ用死亡
	if (Key4.down())
	{
		playerDatas.at(0).player.state = BattleState::lose;
	}

	const double deltaTime = Scene::DeltaTime();

	for (auto i : step((int32)playerCnt))
	{
		auto& playerData = playerDatas.at(i);
		auto& player = playerData.player;

		int32 obstruct = player.update(playerData.keySet);

		if (playerCnt == PlayerCount::By2 && obstruct > 0)
		{
			if (Setting::debugPrint)	Print << U"send obstructs.";
			playerDatas.at((i + 1) % 2).player.sendObstructs(obstruct, player.atkRate);
		}

		// 負けと表示する演出
		if (player.state == BattleState::lose)
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

	if (Setting::debugPrint)	Print << U"end Battle::update()";
}

void Battle::draw() const
{
	if (Setting::debugPrint)	Print << U"in Battle::draw()";

	Rect(0, (int32)(Scene::Height() * 0.7), Scene::Width(), (int32)(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	for (auto& playerData : playerDatas)
	{
		auto& player = playerData.player;

		player.draw(playerData.fieldPos, cellSize);

		// スコア
		FontAsset(U"Text")(U"Score:{}"_fmt(player.score))
			.drawAt(playerData.fieldPos.movedBy(player.fieldSize.x * cellSize.x / 2, player.fieldSize.y * cellSize.y + 30), ColorF(0.25));
		// コンボ
		FontAsset(U"Text")(U"Combo:{}"_fmt(player.combo))
			.drawAt(playerData.fieldPos.movedBy(player.fieldSize.x * cellSize.x / 2, player.fieldSize.y * cellSize.y + 75), ColorF(0.25));

		FontAsset(U"Text")(U"obstructsMaked:{}"_fmt(player.obstructsMaked))
			.drawAt(playerData.fieldPos.movedBy(player.fieldSize.x * cellSize.x / 2, player.fieldSize.y + 60), ColorF(0.25));
		FontAsset(U"Text")(U"obstructsSentSum:{}"_fmt(player.obstructsSentSum))
			.drawAt(playerData.fieldPos.movedBy(player.fieldSize.x * cellSize.x / 2, player.fieldSize.y + 105), ColorF(0.25));

		// 負けの知らせ
		if (playerData.player.state != BattleState::playing)
		{
			const double e = EaseOutBounce(player.loseTimer / player.loseCoolTime <= 1.0 ? player.loseTimer / player.loseCoolTime : 1.0);
			const Vec2 to(playerData.fieldPos + player.fieldSize * cellSize / 2.0);
			const Vec2 pos(Vec2(playerData.fieldPos.x + player.fieldSize.x * cellSize.x / 2.0, -100).lerp(to, e));
			String text = U"";
			if (player.state == BattleState::win)	text = U"Win";
			if (player.state == BattleState::tie)	text = U"Tie";
			if (player.state == BattleState::lose)	text = U"Lose";
			FontAsset(U"Header")(text).drawAt(pos, Palette::Black);
		}
	}

	// タイトルに戻るカウントダウン
	if (isFinished)
	{
		FontAsset(U"Text")(U"タイトルへ戻るまで{:.0f}s"_fmt(Max(backTime - backTimer, 0.0)))
			.drawAt(Scene::Center(), ColorF(0.0));
	}

	if (Setting::debugPrint)	Print << U"end Battle::draw()";
}
