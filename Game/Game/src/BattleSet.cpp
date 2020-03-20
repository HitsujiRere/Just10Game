
# include "BattleSet.hpp"

BattleSet::BattleSet(const InitData& init)
	: IScene(init)
{
	const INIData ini(Resource(U"charData.ini"));
	if (ini)
	{
		// 全てのセクションを列挙
		for (const auto& section : ini.sections())
		{
			// セクション名
			Print << U"[{}]"_fmt(section.section);

			// セクション内のすべてのキーを列挙
			for (const auto& key : section.keys)
			{
				// キーの名前と値
				Print << U"{} = {}"_fmt(key.name, key.value);
			}
		}
		Print << U"-----";
	}
	else
	{
		Print <<  U"Cannot read Resource('charData.ini')!";
		//descs.push_back(U"Cannot read 'HowTo_desc.txt'!");
	}
}

void BattleSet::update()
{
	// タイトルへ戻る
	if (backKeys.down())
	{
		changeScene(State::Title);
	}

	changeScene(State::Battle);
}

void BattleSet::draw() const
{
	Rect(0, static_cast<int32>(Scene::Height() * 0.7), Scene::Width(), static_cast<int32>(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));
}
