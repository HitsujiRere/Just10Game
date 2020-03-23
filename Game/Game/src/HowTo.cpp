
# include "HowTo.hpp"

HowTo::HowTo(const InitData& init)
	: IScene(init)
{
	TextReader reader(Resource(U"texts/HowTo_desc.txt"));
	if (reader)
	{
		desc = U"";
		String line;

		while (reader.readLine(line))
		//for (int32 i = 0; reader.readLine(line); ++i)
		{
			desc += line + U"\n";
			//descs.push_back(line);
		}
	}
	else
	{
		Print <<  U"Cannot read Resource('HowTo_desc.txt')!";
		//descs.push_back(U"Cannot read 'HowTo_desc.txt'!");
	}
}

void HowTo::update()
{
	if (KeysBack.down())
	{
		changeScene(State::Title);
	}
}

void HowTo::draw() const
{
	// タイトルの表示
	{
		const String headerText = U"あそびかた";
		const Vec2 center(Scene::Center().x, 80);
		FontAsset(U"Header")(headerText).drawAt(center.movedBy(4, 6), ColorF(0.0, 0.4));
		FontAsset(U"Header")(headerText).drawAt(center, ColorF(0.2));
	}

	FontAsset(U"Desc")(desc).draw(Arg::topCenter(Scene::Center().x, 200), ColorF(0.25));
}
