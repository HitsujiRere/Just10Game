
# include "BattleSet.hpp"

BattleSet::BattleSet(const InitData& init)
	: IScene(init)
{
	//const Array<Character>& characters = Character::getCharacters();
	auto& playerDatas = Battle::playerDatas;
	int32 playerCnt = static_cast<int32>(getData().playerCnt);

	playerDatas.resize(playerCnt);

	if (getData().playerCnt == PlayerCount::By1)
	{
		const KeyGroup moveL = (KeyA | KeyLeft);
		const KeyGroup moveR = (KeyD | KeyRight);
		const KeyGroup drop = (KeyS | KeyDown);
		const KeyGroup hold = (KeyW | KeyUp);
		playerDatas.at(0).keySet = PlayerKeySet(moveL, moveR, drop, hold);
	}

	if (getData().playerCnt == PlayerCount::By2)
	{
		{
			const KeyGroup moveL = (KeyA | Key());
			const KeyGroup moveR = (KeyD | Key());
			const KeyGroup drop = (KeyS | Key());
			const KeyGroup hold = (KeyW | Key());
			playerDatas.at(0).keySet = PlayerKeySet(moveL, moveR, drop, hold);
		}
		{
			const KeyGroup moveL = (KeyLeft | Key());
			const KeyGroup moveR = (KeyRight | Key());
			const KeyGroup drop = (KeyDown | Key());
			const KeyGroup hold = (KeyUp | Key());
			playerDatas.at(1).keySet = PlayerKeySet(moveL, moveR, drop, hold);
		}
	}

	charactersChoiseNum.resize(playerCnt);
	for (auto i : step(playerCnt))
	{
		charactersChoiseNum.at(i) = playerDatas.at(i).characterNum;
	}

	charactersChoiseEnd.resize(playerCnt);

	{
		TextReader reader(Resource(U"texts/HowTo_Play.txt"));

		if (reader)
		{
			playDescs.resize(1);

			int32 i = 0;
			String line;

			while (reader.readLine(line))
			{
				if (line.isEmpty())
				{
					++i;
					playDescs.push_back(U"");
				}
				else
				{
					playDescs.at(i) += line + U"\n";
				}
			}
		}
	}
	}

void BattleSet::update()
{
	const double deltaTime = Scene::DeltaTime();
	const Array<Character>& characters = Character::getCharacters();
	const int32& playerCnt = static_cast<int32>(getData().playerCnt);

	// タイトルへ戻る
	if (KeysBack.down())
	{
		changeScene(State::Title);
	}

	if (KeyEnter.down())
	{
		changeScene(State::Battle);
	}

	for (auto playerNum : step(playerCnt))
	{
		auto& playerData = Battle::playerDatas.at(playerNum);
		const auto& keySets = playerData.keySet;

		if (keySets.KeysLeft.down() && charactersChoiseNum.at(playerNum) > 0)
		{
			charactersChoiseNum.at(playerNum)--;
			playerData.characterNum = charactersChoiseNum.at(playerNum);
		}

		if (keySets.KeysRight.down() && charactersChoiseNum.at(playerNum) < characters.size() - 1)
		{
			charactersChoiseNum.at(playerNum)++;
			playerData.characterNum = charactersChoiseNum.at(playerNum);
		}

		if (keySets.KeysDown.down())
		{
			charactersChoiseEnd.at(playerNum) ^= true;
		}
	}

	if (charactersChoiseEnd.all())
	{
		startTimer += deltaTime;

		if (startTimer >= startCoolTime)
		{
			changeScene(State::Battle);
		}
	}
	else
	{
		startTimer = 0.0;
	}
}

void BattleSet::draw() const
{
	const Array<Character>& characters = Character::getCharacters();
	const int32 playerCnt = static_cast<int32>(getData().playerCnt);

	Rect(0, static_cast<int32>(Scene::Height() * 0.7), Scene::Width(), static_cast<int32>(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	for (auto playerNum : step(playerCnt))
	{
		const auto& character = characters.at(Battle::playerDatas.at(playerNum).characterNum);

		const Vec2 size(Scene::Size().x / playerCnt, Scene::Size().y - (128 + 32 + 32 + 64));
		const Vec2 pos(64 + size.x * playerNum, 128 + 32 + 32 + 64);
		const Vec2 imgSize(256, 256);
		const Vec2 imgPos(64 + size.x * playerNum, 128 + 32 + 32 + 64);

		const Rect descRect(imgPos.movedBy(imgSize.x + 32, 80).asPoint(), size.movedBy(-imgSize.x - 32 * 4, -70).asPoint());

		for (auto i : step(-2, 5))
		{
			const auto charaNum = i + charactersChoiseNum.at(playerNum);
			if (0 <= charaNum && charaNum < characters.size())
			{
				const Vec2 choiseSize(128, 128);
				const Vec2 choisePos(size.x / 2.0 - choiseSize.x / 2.0 + i * (choiseSize.x + 36) + pos.x, 64 + 32);

				Rect(Arg::center(choisePos.asPoint()), choiseSize.asPoint())
					.drawShadow(Vec2(9, 15), 10.0, 10.0, ColorF(0.0, 0.4))
					.draw(ColorF(1.0))
					.drawFrame(0.0, 4.0, ColorF(0.2));

				characters.at(charaNum).texture.resized(choiseSize).drawAt(choisePos);

				if (i == 0)
				{
					Triangle(choisePos.movedBy(0, choiseSize.y / 2.0 + 18), 40).draw(ColorF(0.2));
				}
			}
		}

		if (charactersChoiseNum.at(playerNum) - 2 > 0)
		{
			Triangle({ size.x / 2.0 - 128 / 2.0 + -2.5 * (128 + 36) + pos.x, 64 + 32 }, 40, 30_deg).draw(ColorF(0.2));
		}
		if (charactersChoiseNum.at(playerNum) + 2 < characters.size() - 1)
		{
			Triangle({ size.x / 2.0 - 128 / 2.0 + 2.5 * (128 + 36) + pos.x, 64 + 32 }, 40, -30_deg).draw(ColorF(0.2));
		}

		Rect(imgPos.asPoint(), imgSize.asPoint())
			.drawShadow(Vec2(9, 15), 10.0, 10.0, ColorF(0.0, 0.4))
			.draw(ColorF(1.0))
			.drawFrame(0.0, 4.0, ColorF(0.2));

		character.texture.resized(imgSize).draw(imgPos);

		FontAsset(U"Name")(character.name).draw(imgPos.movedBy(imgSize.x + 32, 0), ColorF(0.2));

		FontAsset(U"Desc")(character.desc).draw(descRect, ColorF(0.2));

		String dropCells1LoopTxt = U"1ループのセル : ";
		const int32 sum = character.dropCells1LoopNum.sum();
		for (auto i : step(character.dropCells1LoopNum.size()))
		{
			const auto num = character.dropCells1LoopNum.at(i);
			if (num > 0)
			{
				dropCells1LoopTxt += U"{}={:.0f}%  "_fmt(i, 100.0 * num / sum);
			}
		}
		FontAsset(U"Desc")(dropCells1LoopTxt).draw(imgPos.movedBy(0, imgSize.y + 24), ColorF(0.2));

		FontAsset(U"Desc")(U"ATK = {:.0f}%   DEF = {:.0f}%"_fmt(character.atkRate * 100, character.defRate * 100))
			.draw(imgPos.movedBy(0, imgSize.y + 24 + 40), ColorF(0.2));

		if (charactersChoiseEnd.at(playerNum))
		{
			TextureAsset(U"check-square").drawAt(pos + size / 2, ColorF(0.2));
		}

		if (playerCnt + playerNum - 1 < playDescs.size())
		{
			FontAsset(U"Desc")(playDescs.at(playerCnt + playerNum - 1))
				.draw(imgPos.movedBy(0, imgSize.y + 24 + 40 * 3), ColorF(0.2));
		}
	}

	if (startTimer > 0)
	{
		FontAsset(U"Header")(U"スタートまで{:.1f}秒"_fmt(Max(startCoolTime - startTimer, 0.0))).drawAt(Scene::Center().movedBy(0, -256), ColorF(0.2));
	}
}
