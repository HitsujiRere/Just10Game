
# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene(init)
{
	// ESCキーで終了する
	System::SetTerminationTriggers(System::GetTerminationTriggers() | UserAction::EscapeKeyDown);
}

void Title::update()
{
	placticeTransition.update(placticeButton.mouseOver());
	battleTransition.update(battleButton.mouseOver());
	howtoTransition.update(howtoButton.mouseOver());
	exitTransition.update(exitButton.mouseOver());

	if (placticeButton.mouseOver() || battleButton.mouseOver()
		|| howtoButton.mouseOver() || exitButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (placticeButton.leftClicked())
	{
		// ESCキーで終了しない
		System::SetTerminationTriggers(System::GetTerminationTriggers() & (~UserAction::EscapeKeyDown));

		getData().playerCnt = PlayerCount::By1;
		changeScene(State::BattleSet);
	}

	if (battleButton.leftClicked())
	{
		// ESCキーで終了しない
		System::SetTerminationTriggers(System::GetTerminationTriggers() & (~UserAction::EscapeKeyDown));

		getData().playerCnt = PlayerCount::By2;
		changeScene(State::BattleSet);
	}

	if (howtoButton.leftClicked())
	{
		// ESCキーで終了しない
		System::SetTerminationTriggers(System::GetTerminationTriggers() & (~UserAction::EscapeKeyDown));

		changeScene(State::HowTo);
	}

	if (exitButton.leftClicked())
	{
		System::Exit();
	}
}

void Title::draw() const
{
	Rect(0, static_cast<int32>(Scene::Height() * 0.7), Scene::Width(), static_cast<int32>(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	{
		const String titleTextFront = U"Just ";
		const String titleTextBack = U" Game";
		const Vec2 titlePos(Scene::Center().x, 200);
		const int32 fontsize = static_cast<int32>(FontAsset(U"Title").height() * 0.7);

		//FontAsset(U"Title")(titleText).drawAt(titlePos.movedBy(10, 16), ColorF(0.0, 0.14));
		//FontAsset(U"Title")(titleText).drawAt(titlePos.movedBy(10, 14), ColorF(0.0, 0.14));
		//FontAsset(U"Title")(titleText).drawAt(titlePos.movedBy(8, 16), ColorF(0.0, 0.14));
		//FontAsset(U"Title")(titleText).drawAt(titlePos.movedBy(8, 14), ColorF(0.0, 0.14));
		//FontAsset(U"Title")(titleText).drawAt(titlePos.movedBy(6, 10), ColorF(0.0, 0.4));

		FontAsset(U"Title")(titleTextFront)
			.drawAt(titlePos.movedBy(-FontAsset(U"Title")(titleTextFront).region().w / 2 - fontsize / 2, 0).movedBy(6, 10), ColorF(0.0, 0.4));
		/**/
		{
			const ScopedColorMul2D state(ColorF(0.0, 0.4));
			//Cell::getTexture(CellType::Title10).resized(fontsize).drawAt(titlePos.movedBy(6, 10));
			Cell::getTexture(CellType::Title10).resized(fontsize).drawAt(titlePos.movedBy(0, Periodic::Jump0_1(2s) * -64).movedBy(6, 10));
		}
		/**/
		FontAsset(U"Title")(titleTextBack)
			.drawAt(titlePos.movedBy(FontAsset(U"Title")(titleTextBack).region().w / 2 + fontsize / 2, 0).movedBy(6, 10), ColorF(0.0, 0.4));

		FontAsset(U"Title")(titleTextFront)
			.drawAt(titlePos.movedBy(-FontAsset(U"Title")(titleTextFront).region().w / 2 - fontsize / 2, 0), ColorF(0.2));
		{
			const ScopedColorAdd2D state(ColorF(0.2));
			//Cell::getTexture(CellType::Cell10).resized(fontsize).drawAt(titlePos);
			Cell::getTexture(CellType::Cell10).resized(fontsize).drawAt(titlePos.movedBy(0, Periodic::Jump0_1(2s) * -64));
		}
		FontAsset(U"Title")(titleTextBack)
			.drawAt(titlePos.movedBy(FontAsset(U"Title")(titleTextBack).region().w / 2 + fontsize / 2, 0), ColorF(0.2));

		FontAsset(U"Version")(U"Ver {}.{}.{}"_fmt(Version.x, Version.y, Version.z))
			.draw(Arg::topRight(titlePos.movedBy(FontAsset(U"Title")(titleTextBack).region().w + fontsize / 2, fontsize / 2)), ColorF(0.2));
	}

	placticeButton
		.drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4))
		.draw(ColorF(1.0))
		.draw(ColorF(0.8, placticeTransition.value()))
		.drawFrame(2, ColorF(0.2));
	battleButton
		.drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4))
		.draw(ColorF(1.0))
		.draw(ColorF(0.8, battleTransition.value()))
		.drawFrame(2, ColorF(0.2));
	howtoButton
		.drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4))
		.draw(ColorF(1.0))
		.draw(ColorF(0.8, howtoTransition.value()))
		.drawFrame(2, ColorF(0.2));
	exitButton
		.drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4))
		.draw(ColorF(1.0))
		.draw(ColorF(0.8, exitTransition.value()))
		.drawFrame(2, ColorF(0.2));

	FontAsset(U"Menu")(U"1人で あそぶ").drawAt(placticeButton.center(), ColorF(0.25));
	FontAsset(U"Menu")(U"2人で たたかう").drawAt(battleButton.center(), ColorF(0.25));
	FontAsset(U"Menu")(U"あそびかた").drawAt(howtoButton.center(), ColorF(0.25));
	FontAsset(U"Menu")(U"おわる").drawAt(exitButton.center(), ColorF(0.25));

	FontAsset(U"Score")(U"High Score : {}"_fmt(getData().highScore)).draw(Arg::leftCenter(Scene::Center().movedBy(170, 0)), ColorF(0.25));
	FontAsset(U"Score")(U"Play Time : {}"_fmt(getData().play_time)).draw(Arg::leftCenter(Scene::Center().movedBy(170, 100)), ColorF(0.25));
}
