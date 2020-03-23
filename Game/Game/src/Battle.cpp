
# include "Battle.hpp"

Array<PlayerData> Battle::playerDatas;

Battle::Battle(const InitData& init)
	: IScene(init)
{
	const auto& characters = Character::getCharacters();

	getData().play_time++;

	if (getData().playerCnt == PlayerCount::By1)
	{
		auto player = Player(characters.at(playerDatas.at(0).characterNum).dropCells1LoopNum);
		playerDatas.at(0).player = player;
		playerDatas.at(0).fieldPos = Point(cellSize * Size(4, 3));
		playerDatas.at(0).drawMode = FieldDrawMode::Left;
	}

	if (getData().playerCnt == PlayerCount::By2)
	{
		{
			auto player = Player(characters.at(playerDatas.at(0).characterNum).dropCells1LoopNum);
			playerDatas.at(0).player = player;
			playerDatas.at(0).fieldPos = Point(cellSize * Size(4, 3));
			playerDatas.at(0).drawMode = FieldDrawMode::Left;
		}
		{
			auto player = Player(characters.at(playerDatas.at(1).characterNum).dropCells1LoopNum);
			player.fieldColor = Color(231, 184, 194);
			playerDatas.at(1).player = player;
			playerDatas.at(1).fieldPos = Point(cellSize * Size(4, 3)).movedBy(Scene::Center().x, 0);
			playerDatas.at(1).drawMode = FieldDrawMode::Right;
		}
	}
}

void Battle::update()
{
	// タイトルへ戻る
	if (KeysBack.down())
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
	if (KeyF6.down())
	{
		playerDatas.at(0).player.state = BattleState::lose;
	}

	const double deltaTime = Scene::DeltaTime();

	for (auto i : step(static_cast<int32>(getData().playerCnt)))
	{
		auto& playerData = playerDatas.at(i);
		auto& player = playerData.player;
		auto& characters = Character::getCharacters();
		auto& character = characters.at(playerData.characterNum);
		auto& anotherplayerData = playerDatas.at((i + 1) % 2);

		player.update(playerData.keySet);

		if (getData().playerCnt == PlayerCount::By2 && player.isSendObstruct)
		{
			anotherplayerData.player.sentObstructs(player.sendingObstructCnt * character.atkRate / characters.at(anotherplayerData.characterNum).defRate);
		}

		// 負けと表示する演出
		if (player.state == BattleState::lose)
		{
			isFinished = true;
			backTimer += deltaTime;

			if (getData().playerCnt == PlayerCount::By2)
			{
				anotherplayerData.player.state = BattleState::win;
			}

			if (backTimer > backTime || KeysBack.down())
			{
				changeScene(State::Title);
			}
		}
	}
}

void Battle::draw() const
{
	Rect(0, static_cast<int32>(Scene::Height() * 0.7), Scene::Width(), static_cast<int32>(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	for (auto i : step(static_cast<int32>(getData().playerCnt)))
	{
		const auto& playerData = playerDatas.at(i);
		const auto& player = playerData.player;
		const Size drawsize = player.field.getDrawsize();
		const Point fieldCenter = playerData.fieldPos + drawsize * cellSize / 2;

		player.draw(playerData.fieldPos, cellSize, playerData.drawMode);

		// スコア
		{
			const Point pos(fieldCenter.x, playerData.fieldPos.y + drawsize.y * cellSize.y + 50);
			FontAsset(U"Text")(U"Score:{}"_fmt(player.score)).drawAt(pos.movedBy(2, 3), ColorF(0.0, 0.4));
			FontAsset(U"Text")(U"Score:{}"_fmt(player.score)).drawAt(pos, ColorF(0.25));
		}

		// コンボ
		if (player.combo > 0)
		{
			FontAsset(U"Combo")(U"{}"_fmt(player.combo)).drawAt(fieldCenter, ColorF(0.25, 0.75));
		}
 
		// 送るオジャマ
		if (player.sendingObstructCnt > 0)
		{
			const auto& anotherPlayerData = playerDatas.at((i + 1) % 2);
			const double e = EaseInExpo(Min(player.sendingObstructTimer / player.sendingObstructCoolTime, 1.0));
			const Vec2 from(fieldCenter.x, playerData.fieldPos.y + -cellSize.y * 2);
			const Vec2 to(anotherPlayerData.fieldPos.movedBy(anotherPlayerData.player.field.getDrawsize().x * cellSize.x / 2, -cellSize.y * 2));
			FontAsset(U"Obstruct")(U"{}"_fmt(player.sendingObstructCnt)).drawAt(from.lerp(to, e).moveBy(3, 5), ColorF(0.0, 0.4));
			FontAsset(U"Obstruct")(U"{}"_fmt(player.sendingObstructCnt)).drawAt(from.lerp(to, e), ColorF(0.2));
		}

		/**
		FontAsset(U"Text")(U"Maked:{}"_fmt(player.obstructsMaked))
			.drawAt(playerData.fieldPos.movedBy(drawsize.x * cellSize.x / 2, 100), ColorF(0.25));
		FontAsset(U"Text")(U"SentSum:{}"_fmt(player.obstructsSentSum))
			.drawAt(playerData.fieldPos.movedBy(drawsize.x * cellSize.x / 2, 145), ColorF(0.25));
		/**/

		// 負けの知らせ
		if (playerData.player.state != BattleState::playing)
		{
			const double e = EaseOutBounce(Min(player.stateTimer / player.stateCoolTime, 1.0));
			const Vec2 to(fieldCenter);
			const Vec2 from(fieldCenter.x, -100);
			const String text
				= player.state == BattleState::win ? U"Win"
				: player.state == BattleState::tie ? U"Tie"
				: player.state == BattleState::lose ? U"Lose"
				: U"----";
			FontAsset(U"Header")(text).drawAt(from.lerp(to, e), ColorF(0.2));
			//FontAsset(U"Header")(text).drawAt(to, ColorF(0.2));
		}
	}

	// タイトルに戻るカウントダウン
	if (isFinished)
	{
		FontAsset(U"Text")(U"タイトルへ戻るまで{:.0f}s"_fmt(Max(backTime - backTimer, 0.0)))
			.drawAt(Scene::Center(), ColorF(0.2));
	}
}
