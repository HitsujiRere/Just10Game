
# include "HowTo.hpp"

HowTo::HowTo(const InitData& init)
	: IScene(init)
{
	TextReader reader(Resource(U"Text/HowTo_desc.txt"));
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
		Print <<  U"Cannot read Resource('Text/HowTo_desc.txt')!";
		//descs.push_back(U"Cannot read 'HowTo_desc.txt'!");
	}

	backButton = Rect(Arg::leftCenter(Scene::Center().x + FontAsset(U"Header")(headerText).region().w / 2 + 64, 80), 200, 60);
}

void HowTo::update()
{
	backTransition.update(backButton.mouseOver());

	if (backButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (KeysBack.down() || backButton.leftClicked())
	{
		changeScene(State::Title);
	}
}

void HowTo::draw() const
{
	Rect(0, static_cast<int32>(Scene::Height() * 0.7), Scene::Width(), static_cast<int32>(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.3));

	// タイトルの表示
	{
		const Vec2 center(Scene::Center().x, 80);
		FontAsset(U"Header")(headerText).drawAt(center.movedBy(6, 10), ColorF(0.0, 0.4));
		FontAsset(U"Header")(headerText).drawAt(center, ColorF(0.2));
	}

	backButton
		.drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4))
		.draw(ColorF(1.0))
		.draw(ColorF(0.8, backTransition.value()))
		.drawFrame(2, ColorF(0.2));
	FontAsset(U"Menu")(U"もどる").drawAt(backButton.center(), ColorF(0.25));

	FontAsset(U"Desc")(desc).draw(Arg::topCenter(Scene::Center().x, 200), ColorF(0.25));
}
