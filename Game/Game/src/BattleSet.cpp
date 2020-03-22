
# include "BattleSet.hpp"

BattleSet::BattleSet(const InitData& init)
	: IScene(init)
{
	Battle::playerDatas.resize(static_cast<int32>(getData().playerCnt));

	Battle::playerDatas.each([](PlayerData& data) { data.character = Character::getCharacters().at(0); });
}

void BattleSet::update()
{
	// タイトルへ戻る
	if (backKeys.down())
	{
		changeScene(State::Title);
	}

	if (KeyEnter.down())
	{
		changeScene(State::Battle);
	}

	SimpleGUI::Slider(charactersSlider, Vec2(32, 128 + 32 + 32), 1920 - 32 * 2,
		((static_cast<int32>(Character::getCharacters().size()) - 1)* (128 + 32)) + (64 + 32) > 1920 - (64 + 32));
}

void BattleSet::draw() const
{
	const Array<Character>& characters = Character::getCharacters();

	// SimpleGUI::Slider(charactersSlider, Vec2());

	Rect(0, static_cast<int32>(Scene::Height() * 0.7), Scene::Width(), static_cast<int32>(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	for (int32 i : step(static_cast<int32>(characters.size())))
	{
		const double minX = (i * (128 + 32)) + (64 + 32);
		const double maxX = 1920 - (static_cast<int32>(characters.size()) - i - 1)* (128 + 32) - (64 + 32);
		const Vec2 pos((maxX - minX) * charactersSlider + minX, 64 + 32);

		Rect(Arg::center(pos.asPoint()), 128, 128)
			.drawShadow(Vec2(9, 15), 10.0, 10.0, ColorF(0.0, 0.4))
			.draw(ColorF(1.0))
			.drawFrame(0.0, 4.0, ColorF(0.2));

		characters.at(i).texture.resized(128, 128).drawAt(pos);
	}
}
