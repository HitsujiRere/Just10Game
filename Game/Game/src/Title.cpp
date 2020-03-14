
# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene(init)
{
	// ESCキーで終了する
	System::SetTerminationTriggers(System::GetTerminationTriggers() | UserAction::EscapeKeyDown);
}

void Title::update()
{
	m_placticeTransition.update(m_placticeButton.mouseOver());
	m_battleTransition.update(m_battleButton.mouseOver());
	m_howtoTransition.update(m_howtoButton.mouseOver());
	m_exitTransition.update(m_exitButton.mouseOver());

	if (m_placticeButton.mouseOver() || m_battleButton.mouseOver()
		|| m_howtoButton.mouseOver() || m_exitButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (m_placticeButton.leftClicked())
	{
		// ESCキーで終了しない
		System::SetTerminationTriggers(System::GetTerminationTriggers() & (~UserAction::EscapeKeyDown));

		Battle::playerCnt = PlayerCount::By1;
		changeScene(State::Battle);
	}

	if (m_battleButton.leftClicked())
	{
		// ESCキーで終了しない
		System::SetTerminationTriggers(System::GetTerminationTriggers() & (~UserAction::EscapeKeyDown));

		Battle::playerCnt = PlayerCount::By2;
		changeScene(State::Battle);
	}

	if (m_howtoButton.leftClicked())
	{
		// ESCキーで終了しない
		System::SetTerminationTriggers(System::GetTerminationTriggers() & (~UserAction::EscapeKeyDown));

		changeScene(State::HowTo);
	}

	if (m_exitButton.leftClicked())
	{
		System::Exit();
	}
}

void Title::draw() const
{
	Rect(0, (int32)(Scene::Height() * 0.7), Scene::Width(), (int32)(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

	{
		const String titleText = U"Just 10 Game";
		const Vec2 titlePos(Scene::Center().x, 150);
		FontAsset(U"Title")(titleText).drawAt(titlePos.movedBy(10, 16), ColorF(0.0, 0.14));
		FontAsset(U"Title")(titleText).drawAt(titlePos.movedBy(10, 14), ColorF(0.0, 0.14));
		FontAsset(U"Title")(titleText).drawAt(titlePos.movedBy(8, 16), ColorF(0.0, 0.14));
		FontAsset(U"Title")(titleText).drawAt(titlePos.movedBy(8, 14), ColorF(0.0, 0.14));
		const Rect titleArea = FontAsset(U"Title")(titleText).drawAt(titlePos, ColorF(0.2));
		FontAsset(U"Version")(U"Ver {}.{}.{}"_fmt(Version.x, Version.y, Version.z)).draw(Arg::topRight(titlePos.movedBy(titleArea.size.x / 2, 120)));
	}

	m_placticeButton
		.drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4))
		.draw(ColorF(1.0))
		.draw(ColorF(0.8, m_placticeTransition.value()))
		.drawFrame(2, ColorF(0.2));
	m_battleButton
		.drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4))
		.draw(ColorF(1.0))
		.draw(ColorF(0.8, m_battleTransition.value()))
		.drawFrame(2, ColorF(0.2));
	m_howtoButton
		.drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4))
		.draw(ColorF(1.0))
		.draw(ColorF(0.8, m_howtoTransition.value()))
		.drawFrame(2, ColorF(0.2));
	m_exitButton
		.drawShadow(Vec2(9, 15), 10.0, 0.0, ColorF(0.0, 0.4))
		.draw(ColorF(1.0))
		.draw(ColorF(0.8, m_exitTransition.value()))
		.drawFrame(2, ColorF(0.2));

	FontAsset(U"Menu")(U"1人で あそぶ").drawAt(m_placticeButton.center(), ColorF(0.25));
	FontAsset(U"Menu")(U"2人で たたかう").drawAt(m_battleButton.center(), ColorF(0.25));
	FontAsset(U"Menu")(U"あそびかた").drawAt(m_howtoButton.center(), ColorF(0.25));
	FontAsset(U"Menu")(U"おわる").drawAt(m_exitButton.center(), ColorF(0.25));

	FontAsset(U"Score")(U"High Score : {}"_fmt(getData().highScore)).draw(Arg::leftCenter(Scene::Center().movedBy(170, 0)), ColorF(0.25));
	FontAsset(U"Score")(U"Play Time : {}"_fmt(getData().playTime)).draw(Arg::leftCenter(Scene::Center().movedBy(170, 100)), ColorF(0.25));
}
