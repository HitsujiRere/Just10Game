
//
// Just10Game  ver 0.4.0
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
	Setting::debugPrint = false;

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
	FontAsset::Register(U"Title", 200, U"example/font/AnnyantRoman/AnnyantRoman.ttf");
	FontAsset::Register(U"Version", 60, Typeface::Black);
	FontAsset::Register(U"Menu", 30, Typeface::Regular);
	FontAsset::Register(U"Score", 36, Typeface::Bold);
	FontAsset::Register(U"Header", 100, Typeface::Bold);
	FontAsset::Register(U"Text", 45, Typeface::Regular);
	FontAsset::Register(U"Desc", 30, Typeface::Regular);

	// 同じ形式かどうかはloadVersionで判定
	constexpr int32 load_version = 1;
	// GameDataをロード
	std::shared_ptr<GameData> load_data(new GameData());
	{
		BinaryReader reader(U"GameData.bin");
		if (reader)
		{
			int32 readedLoadVer;
			reader.read(readedLoadVer);
			if (readedLoadVer == load_version)
			{
				reader.read(*load_data);
			}
			else
			{
				if (Setting::debugPrint)	Print << U"Cannot read 'GameData.bin'!";
			}
		}
	}

	// シーンと遷移時の色を設定
	MyApp manager(load_data);
	manager
		.add<Title>(State::Title)
		.add<HowTo>(State::HowTo)
		.add<Battle>(State::Battle)
		.setFadeColor(ColorF(1.0));

	bool isRun = true;
	while (System::Update())
	{
		try
		{
			if (KeyF2.down())
			{
				isRun ^= true;
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

			if (isRun && !manager.update())
			{
				break;
			}

		}
		catch (std::exception & e)
		{
			Print << Unicode::Widen(e.what()) << U" in 'while (System::Update())'.";

			isRun = false;
		}
	}

	// GameDataを保存
	{
		BinaryWriter writter(U"GameData.bin");
		if (writter)
		{
			writter.write(load_version);
			writter.write(*manager.get());
		}
	}
}
