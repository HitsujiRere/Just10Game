
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
		{
			desc += line + U"\n";
		}
	}
	else
	{
		desc = U"Cannot read 'HowTo_desc.txt'!";
	}
}

void HowTo::update()
{
	if (backKeys.down())
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
		FontAsset(U"Header")(headerText).drawAt(center, Color(0.2));
	}

	FontAsset(U"Desc")(desc).draw(Arg::topCenter(Scene::Center().x, 200), ColorF(0.25));

	Rect(0, (int32)(Scene::Height() * 0.7), Scene::Width(), (int32)(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));
}
