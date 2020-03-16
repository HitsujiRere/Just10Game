
# include "Battle.hpp"

PlayerCount Battle::playerCnt = PlayerCount::By1;

Battle::Battle(const InitData& init)
	: IScene(init)
{
	getData().play_time++;

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
	if (Setting::debugPrint)	Print << U"Battle::update begin";

	// タイトルへ戻る
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

	// デバッグ用死亡
	if (Key4.down())
	{
		playerDatas.at(0).player.state = BattleState::lose;
	}

	const double deltaTime = Scene::DeltaTime();

	for (auto i : step(static_cast<int32>(playerCnt)))
	{
		auto& playerData = playerDatas.at(i);
		auto& player = playerData.player;

		if (Setting::debugPrint)	Print << U"\tplayer {} call update() before"_fmt(i);
		int32 obstruct = player.update(playerData.keySet);
		if (Setting::debugPrint)	Print << U"\tplayer {} call update() after"_fmt(i);

		if (playerCnt == PlayerCount::By2 && obstruct > 0)
		{
			if (Setting::debugPrint)	Print << U"\t- 0 - 1";
			if (Setting::debugPrint)	Print << U"send obstructs.";
			if (Setting::debugPrint)	Print << U"\t- 0 - 2";
			playerDatas.at((i + 1) % 2).player.sendObstructs(obstruct * player.atkRate);
			if (Setting::debugPrint)	Print << U"\t- 0 - 3";
		}

		if (Setting::debugPrint)	Print << U"\t- 1";

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

		if (Setting::debugPrint)	Print << U"\t- 2";
	}

	if (Setting::debugPrint)	Print << U"Battle::update end";
}

void Battle::draw() const
{
	if (Setting::debugPrint)	Print << U"Battle::draw begin";

	Rect(0, static_cast<int32>(Scene::Height() * 0.7), Scene::Width(), static_cast<int32>(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	int32 i = -1;
	for (auto& playerData : playerDatas)
	{
		++i;
		auto& player = playerData.player;
		const Size drawsize = player.field.getDrawsize();

		if (Setting::debugPrint)	Print << U"\tplayer {} call draw() before"_fmt(i);
		player.draw(playerData.fieldPos, cellSize);
		if (Setting::debugPrint)	Print << U"\tplayer {} call draw() after"_fmt(i);

		if (Setting::debugPrint)	Print << U"\t- 1";

		// スコア
		FontAsset(U"Text")(U"Score:{}"_fmt(player.score))
			.drawAt(playerData.fieldPos.movedBy(drawsize.x * cellSize.x / 2, drawsize.y * cellSize.y + 30), ColorF(0.25));

		if (Setting::debugPrint)	Print << U"\t- 2";

		// コンボ
		FontAsset(U"Text")(U"Combo:{}"_fmt(player.combo))
			.drawAt(playerData.fieldPos.movedBy(drawsize.x * cellSize.x / 2, drawsize.y * cellSize.y + 75), ColorF(0.25));

		if (Setting::debugPrint)	Print << U"\t- 3";

		FontAsset(U"Text")(U"Maked:{}"_fmt(player.obstructsMaked))
			.drawAt(playerData.fieldPos.movedBy(drawsize.x * cellSize.x / 2, 100), ColorF(0.25));
		FontAsset(U"Text")(U"SentSum:{}"_fmt(player.obstructsSentSum))
			.drawAt(playerData.fieldPos.movedBy(drawsize.x * cellSize.x / 2, 145), ColorF(0.25));


		// 負けの知らせ
		if (playerData.player.state != BattleState::playing)
		{
			const double e = EaseOutBounce(player.loseTimer / player.loseCoolTime <= 1.0 ? player.loseTimer / player.loseCoolTime : 1.0);
			const Vec2 to(playerData.fieldPos + drawsize * cellSize / 2.0);
			const Vec2 pos(Vec2(playerData.fieldPos.x + drawsize.x * cellSize.x / 2.0, -100).lerp(to, e));
			String text = U"";
			if (player.state == BattleState::win)	text = U"Win";
			if (player.state == BattleState::tie)	text = U"Tie";
			if (player.state == BattleState::lose)	text = U"Lose";
			FontAsset(U"Header")(text).drawAt(pos, Palette::Black);
		}

		if (Setting::debugPrint)	Print << U"\t- 4";
	}

	// タイトルに戻るカウントダウン
	if (isFinished)
	{
		FontAsset(U"Text")(U"タイトルへ戻るまで{:.0f}s"_fmt(Max(backTime - backTimer, 0.0)))
			.drawAt(Scene::Center(), ColorF(0.0));
	}

	if (Setting::debugPrint)	Print << U"Battle::draw end";
}
