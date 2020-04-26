
# include "Battle.hpp"

Array<PlayerData> Battle::playerDatas;

Battle::Battle(const InitData& init)
	: IScene(init)
{
	const auto& characters = Character::getCharacters();

	getData().playTime++;

	if (getData().playerCnt == PlayerCount::By1)
	{
		auto playerPtr = std::make_shared<Player>(characters.at(playerDatas.at(0).characterNum).dropCells1LoopNum);
		playerDatas.at(0).playerPtr = std::shared_ptr<Player>(playerPtr);
		playerDatas.at(0).fieldPos = Point(cellSize * Size(4, 3));
		playerDatas.at(0).drawMode = FieldDrawMode::Left;
	}

	if (getData().playerCnt == PlayerCount::By2)
	{
		{
			auto playerPtr = std::make_shared<Player>(characters.at(playerDatas.at(0).characterNum).dropCells1LoopNum);
			playerDatas.at(0).playerPtr = std::shared_ptr<Player>(playerPtr);
			playerDatas.at(0).fieldPos = Point(cellSize * Size(4, 3));
			playerDatas.at(0).drawMode = FieldDrawMode::Left;

			if (getData().playersType.at(0) == PlayerType::Auto)
			{
				auto op = std::dynamic_pointer_cast<PlayerOperatorAuto>(playerDatas.at(0).operaterPtr);
				op->setPlayer(playerPtr);
			}
		}
		{
			auto playerPtr = std::make_shared<Player>(characters.at(playerDatas.at(1).characterNum).dropCells1LoopNum);
			playerPtr->fieldColor = Color(231, 184, 194);
			playerDatas.at(1).playerPtr = std::shared_ptr<Player>(playerPtr);
			playerDatas.at(1).fieldPos = Point(cellSize * Size(4, 3)).movedBy(Scene::Center().x, 0);
			playerDatas.at(1).drawMode = FieldDrawMode::Right;

			if (getData().playersType.at(1) == PlayerType::Auto)
			{
				auto op = std::dynamic_pointer_cast<PlayerOperatorAuto>(playerDatas.at(1).operaterPtr);
				op->setPlayer(playerPtr);
			}
		}
	}
}

void Battle::update()
{
	titleTransition.update(titleButton.mouseOver());

	if (titleButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	// タイトルへ戻る
	if (KeysBack.down() || titleButton.leftClicked())
	{
		int32 hightScore = getData().highScore;
		for (auto& playerData : playerDatas)
		{
			hightScore = Max(hightScore, playerData.playerPtr->score);
		}
		getData().highScore = hightScore;
		changeScene(State::Title);
	}

	// デバッグ用死亡
	if (KeyF6.down())
	{
		playerDatas.at(0).playerPtr->state = BattleState::lose;
	}

	const double deltaTime = Scene::DeltaTime();

	for (auto i : step(static_cast<int32>(getData().playerCnt)))
	{
		auto& playerData = playerDatas.at(i);
		auto& player = *playerData.playerPtr;
		auto& characters = Character::getCharacters();
		auto& character = characters.at(playerData.characterNum);

		playerData.operaterPtr->update();

		player.update(playerData.operaterPtr);

		if (getData().playerCnt == PlayerCount::By2 && player.isSendObstruct)
		{
			auto& anotherplayerData = playerDatas.at((i + 1) % 2);
			anotherplayerData.playerPtr->sentObstructs(player.sendingObstructCnt * character.atkRate / characters.at(anotherplayerData.characterNum).defRate);
		}

		// 負けと表示する演出
		if (player.state == BattleState::lose)
		{
			isFinished = true;
			backTimer += deltaTime;

			if (getData().playerCnt == PlayerCount::By2)
			{
				auto& anotherplayerData = playerDatas.at((i + 1) % 2);
				anotherplayerData.playerPtr->state = BattleState::win;
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
		const auto& player = *playerData.playerPtr;
		const Size drawsize = player.field.getDrawsize();
		const Point fieldCenter = playerData.fieldPos + drawsize * cellSize / 2;

		player.draw(playerData, cellSize);
 
		// 送るオジャマ
		if (getData().playerCnt == PlayerCount::By2 && player.sendingObstructCnt > 0)
		{
			const auto& anotherPlayerData = playerDatas.at((i + 1) % 2);
			const double e = EaseInExpo(Min(player.sendingObstructTimer / player.sendingObstructCoolTime, 1.0));
			const Vec2 from(fieldCenter.x, playerData.fieldPos.y + -cellSize.y * 2);
			const Vec2 to(anotherPlayerData.fieldPos.movedBy(anotherPlayerData.playerPtr->field.getDrawsize().x * cellSize.x / 2, -cellSize.y * 2));
			FontAsset(U"Obstruct")(U"{}"_fmt(player.sendingObstructCnt)).drawAt(from.lerp(to, e).moveBy(3, 5), ColorF(0.0, 0.4));
			FontAsset(U"Obstruct")(U"{}"_fmt(player.sendingObstructCnt)).drawAt(from.lerp(to, e), ColorF(0.2));
		}
	}

	// タイトルに戻るカウントダウン
	if (isFinished)
	{
		FontAsset(U"Text")(U"タイトルへ戻るまで{:.0f}s"_fmt(Max(backTime - backTimer, 0.0)))
			.drawAt(Scene::Center(), ColorF(0.2));
	}

	// タイトルへ戻るボタン
	titleButton
		.drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4))
		.draw(ColorF(1.0))
		.draw(ColorF(0.8, titleTransition.value()))
		.drawFrame(2, ColorF(0.2));
	FontAsset(U"Menu")(U"もどる").drawAt(titleButton.center(), ColorF(0.25));
}
