
# include "BattleSet.hpp"

BattleSet::BattleSet(const InitData& init)
	: IScene(init)
{
	//const Array<Character>& characters = Character::getCharacters();
	auto& playerDatas = Battle::playerDatas;

	playerDatas.resize(static_cast<int32>(getData().playerCnt));

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

	charactersChoiseNum.resize(static_cast<int32>(getData().playerCnt));
	for (auto i : step(static_cast<int32>(getData().playerCnt)))
	{
		charactersChoiseNum.at(i) = playerDatas.at(i).characterNum;
	}

	charactersChoiseEnd.resize(static_cast<int32>(getData().playerCnt));
}

void BattleSet::update()
{
	const Array<Character>& characters = Character::getCharacters();
	const int32& playerCnt = static_cast<int32>(getData().playerCnt);

	// タイトルへ戻る
	if (backKeys.down())
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
		const auto& keySet = playerData.keySet;

		if (keySet.moveL.down() && charactersChoiseNum.at(playerNum) > 0)
		{
			charactersChoiseNum.at(playerNum)--;
			playerData.characterNum = charactersChoiseNum.at(playerNum);
		}

		if (keySet.moveR.down() && charactersChoiseNum.at(playerNum) < characters.size() - 1)
		{
			charactersChoiseNum.at(playerNum)++;
			playerData.characterNum = charactersChoiseNum.at(playerNum);
		}
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

		TextureAsset(U"check-square").drawAt(pos + size / 2, ColorF(0.2));
	}
}
