
//
// Just10Game
// made by HitsujiRere
//

//
// macOS では README.md と同じフォルダに
// include と lib をコピーしてください。
//

# include "Common.hpp"
# include "Title.hpp"
# include "HowTo.hpp"
# include "Battle.hpp"

void Main()
{
	// （ウィンドウの閉じるボタンを押したときだけ終了する場合はコメントを外す）
	//System::SetTerminationTriggers(UserAction::CloseButtonClicked);

	// タイトルを設定
	Window::SetTitle(U"Just10Game");

	// 背景色を設定
	Scene::SetBackground(ColorF(0.2, 0.8, 0.4));

	// 使用するフォントアセットを登録
	FontAsset::Register(U"Title", 100, U"example/font/AnnyantRoman/AnnyantRoman.ttf");
	FontAsset::Register(U"Menu", 30, Typeface::Regular);
	FontAsset::Register(U"Score", 36, Typeface::Bold);
	FontAsset::Register(U"Text", 30, Typeface::Regular);
	FontAsset::Register(U"Header", 50, Typeface::Bold);

	// シーンと遷移時の色を設定
	MyApp manager;
	manager
		.add<Title>(State::Title)
		.add<HowTo>(State::HowTo)
		.add<Battle>(State::Battle)
		.setFadeColor(ColorF(1.0));

	// （ゲームシーンから開始する場合はコメントを外す）
	//manager.init(State::Game);

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}
