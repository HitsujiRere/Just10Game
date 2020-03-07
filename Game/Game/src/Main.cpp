
//
// Just10Game  ver 0.1.0
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
	// ESCキーで終了しない
	System::SetTerminationTriggers(System::GetTerminationTriggers() & (~UserAction::EscapeKeyDown));

	// タイトルを設定
	Window::SetTitle(U"Just10Game");
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
	Window::Resize(windowSizes[windowSizeNum]);
	// 手動でリサイズできるように変更
	Window::SetStyle(WindowStyle::Sizable);

	// 背景色を設定
	Scene::SetBackground(ColorF(1.0));

	// 使用するフォントアセットを登録
	FontAsset::Register(U"Title", 200, U"example/font/AnnyantRoman/AnnyantRoman.ttf");
	FontAsset::Register(U"Version", 60, Typeface::Regular);
	FontAsset::Register(U"Menu", 30, Typeface::Regular);
	FontAsset::Register(U"Score", 36, Typeface::Bold);
	FontAsset::Register(U"Header", 100, Typeface::Bold);
	FontAsset::Register(U"Text", 45, Typeface::Regular);
	FontAsset::Register(U"Desc", 30, Typeface::Regular);

	// 同じ形式かどうかはloadVersionで判定
	constexpr int32 loadVersion = 1;
	// GameDataをロード
	std::shared_ptr<GameData> loadData(new GameData());
	{
		BinaryReader reader(U"GameData.bin");
		if (reader)
		{
			int32 readedLoadVer;
			reader.read(readedLoadVer);
			if (readedLoadVer == loadVersion)
			{
				reader.read(*loadData);
			}
			else
			{
				//Print << U"Cannot read 'GameData.bin'!";
			}
		}
	}

	// シーンと遷移時の色を設定
	MyApp manager(loadData);
	manager
		.add<Title>(State::Title)
		.add<HowTo>(State::HowTo)
		.add<Battle>(State::Battle)
		.setFadeColor(ColorF(1.0));

	while (System::Update())
	{
		if (KeyF5.down())
		{
			windowSizeNum = ++windowSizeNum % windowSizes.size();
			Window::Resize(windowSizes[windowSizeNum], WindowResizeOption::KeepSceneSize);
		}

		if (KeyF4.down())
		{
			isFullscreen ^= true;
			const bool isSetFullscreen = Window::SetFullscreen(isFullscreen);
			Scene::Resize(windowSizes[0]);
		}

		if (!manager.update())
		{
			break;
		}
	}

	{
		BinaryWriter writter(U"GameData.bin");
		if (writter)
		{
			writter.write(loadVersion);
			writter.write(*manager.get());
		}
	}
}
