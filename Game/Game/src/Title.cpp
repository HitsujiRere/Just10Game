
# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene(init)
{

}

void Title::update()
{
	m_startTransition.update(m_startButton.mouseOver());
	m_howtoTransition.update(m_howtoButton.mouseOver());
	m_exitTransition.update(m_exitButton.mouseOver());

	if (m_startButton.mouseOver() || m_howtoButton.mouseOver()  ||m_exitButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (m_startButton.leftClicked())
	{
		changeScene(State::Battle);
	}

	if (m_howtoButton.leftClicked())
	{
		changeScene(State::HowTo);
	}

	if (m_exitButton.leftClicked())
	{
		System::Exit();
	}
}

void Title::draw() const
{
	const String titleText = U"Just 10 Game";
	const Vec2 center(Scene::Center().x, 150);
	FontAsset(U"Title")(titleText).drawAt(center.movedBy(6, 9), ColorF(0.0, 0.5));
	FontAsset(U"Title")(titleText).drawAt(center);

	m_startButton.draw(ColorF(1.0, m_startTransition.value())).drawFrame(2);
	m_howtoButton.draw(ColorF(1.0, m_howtoTransition.value())).drawFrame(2);
	m_exitButton.draw(ColorF(1.0, m_exitTransition.value())).drawFrame(2);

	FontAsset(U"Menu")(U"はじめる").drawAt(m_startButton.center(), ColorF(0.25));
	FontAsset(U"Menu")(U"あそびかた").drawAt(m_howtoButton.center(), ColorF(0.25));
	FontAsset(U"Menu")(U"おわる").drawAt(m_exitButton.center(), ColorF(0.25));

	FontAsset(U"Score")(U"High Score: {}"_fmt(getData().highScore)).draw(Arg::leftCenter(Scene::Center().movedBy(170, 0)));
	FontAsset(U"Score")(U"Play Time : {}"_fmt(getData().playTime)).draw(Arg::leftCenter(Scene::Center().movedBy(170, 100)));

	Rect(0, (int32)(Scene::Height() * 0.7), Scene::Width(), (int32)(Scene::Height() * 0.3))
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));
}
