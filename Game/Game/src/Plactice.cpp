
# include "Plactice.hpp"

Plactice::Plactice(const InitData& init)
	: IScene(init)
{
	getData().playTime++;

	{
		auto player = Player();
		const KeyGroup moveL = (KeyA | KeyLeft);
		const KeyGroup moveR = (KeyD | KeyRight);
		const KeyGroup drop = (KeyS | KeyDown);
		const KeyGroup hold = (KeyW | KeyUp);
		playerData = PlayerData(player, PlayerKeySet(moveL, moveR, drop, hold), Point(cellSize * Size(4, 2)));
	}
}

void Plactice::update()
{
	const double deltaTime = Scene::DeltaTime();

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
		getData().highScore = Max(getData().highScore, playerData.player.score);
		changeScene(State::Title);
	}
}

void Plactice::draw() const
{
	Rect(0, (int32)(Scene::Height() * 0.7), Scene::Width(), (int32)(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	{
		playerData.player.draw(playerData.fieldPos, cellSize);

		// スコア
		FontAsset(U"Text")(U"Score:{}"_fmt(playerData.player.score)).drawAt(playerData.fieldPos.movedBy(playerData.player.fieldSize.x * cellSize.x / 2, playerData.player.fieldSize.y * cellSize.y + 30), ColorF(0.25));
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
