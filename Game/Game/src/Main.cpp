
//
// Just10Game  ver 1.0.0
// made by HitsujiRere
//

//
// macOS では README.md と同じフォルダに
// include と lib をコピーしてください。
//

# include "Common.hpp"
# include "Title.hpp"
# include "HowTo.hpp"
# include "BattleSet.hpp"
# include "Battle.hpp"

void Main()
{
	// タイトルを設定
	Window::SetTitle(U"Just10Game Ver {}.{}.{}"_fmt(Version.x, Version.y, Version.z));
	// ウィンドウサイズ
	const Array<Size> windowSizes
	{
		Size(1920, 1080),	// 1.00
		Size(1440, 810),	// 0.75
		Size(960, 540),		// 0.50
		Size(480, 270),		// 0.25
	};
	// 現在のウィンドウサイズ番号
	int32 windowSizeNum = 0;
	// フルスクリーンかどうか
	bool isFullscreen = false;
	Window::Resize(windowSizes.at(windowSizeNum));
	// 手動でリサイズできるように変更
	Window::SetStyle(WindowStyle::Sizable);

	// 背景色を設定
	Scene::SetBackground(ColorF(1.0));

	// 使用するフォントアセットを登録
	FontAsset::Register(U"Title", 200, Resource(U"Font/Black_Ops_One/BlackOpsOne-Regular.ttf"));
	FontAsset::Register(U"Version", 60, Typeface::Black);
	FontAsset::Register(U"Menu", 30, Typeface::Regular);
	FontAsset::Register(U"Score", 36, Typeface::Bold);
	FontAsset::Register(U"Header", 150, Typeface::Heavy);
	FontAsset::Register(U"Text", 45, Typeface::Regular);
	FontAsset::Register(U"Desc", 30, Typeface::Regular);
	FontAsset::Register(U"Combo", 200, Typeface::Heavy);
	FontAsset::Register(U"Obstruct", 100, Typeface::Heavy);
	FontAsset::Register(U"Name", 60, Typeface::Regular);

	// 使用するテクスチャーアセット
	TextureAsset::Register(U"check-square", Icon(0xf14a, 500));

	// 同じ形式かどうかはloadVersionで判定
	constexpr int32 load_version = 1;
	// GameDataをロード
	std::shared_ptr<GameData> gamedataPtr(new GameData());
	gamedataPtr->loadSetting(U"data/setting.ini");
	bool isSaveGameData = gamedataPtr->loadGameData(U"data/GameData.bin");

	Character::loadCharacters(gamedataPtr->charaPath, gamedataPtr->charaResourcePath);

	// シーンと遷移時の色を設定
	MyApp manager(gamedataPtr);
	manager
		.add<Title>(State::Title)
		.add<HowTo>(State::HowTo)
		.add<BattleSet>(State::BattleSet)
		.add<Battle>(State::Battle)
		.setFadeColor(ColorF(1.0));

	bool callUpdate = true;
	while (System::Update())
	{
		try
		{
			if (KeyF2.down())
			{
				callUpdate ^= true;
			}

			if (KeyF3.down())
			{
				gamedataPtr->debug ^= true;
			}

			// 画面サイズ変更
			if (KeyF5.down())
			{
				windowSizeNum = ++windowSizeNum % windowSizes.size();
				Window::Resize(windowSizes.at(windowSizeNum), WindowResizeOption::KeepSceneSize);
			}

			// フルスクリーン変更
			if (KeyF4.down())
			{
				isFullscreen ^= true;
				const bool isSetFullscreen = Window::SetFullscreen(isFullscreen);
				Scene::Resize(windowSizes.at(0));
			}

			if (callUpdate && !manager.update())
			{
				break;
			}

			if (gamedataPtr->debug)
			{
				for (auto i : step(1, 7))
				{
					const auto p = Scene::Size() * i / 8;
					Line(p.x, 0.0, p.x, Scene::Size().y).draw(Palette::Red);
					Line(0.0, p.y, Scene::Size().x, p.y).draw(Palette::Red);
				}
			}
		}
		catch (const Error & e)
		{
			Print << e.what() << U" in 'while (System::Update())'.";
			callUpdate = false;
		}
	}

	// GameDataを保存
	if (isSaveGameData)
	{
		gamedataPtr->saveGameData(U"data/GameData.bin");
	}
}
