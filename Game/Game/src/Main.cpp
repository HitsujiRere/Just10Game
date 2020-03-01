
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
	// ESCキーで終了しない
	System::SetTerminationTriggers(System::GetTerminationTriggers() & (~UserAction::EscapeKeyDown));

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

	// 同じ形式かどうかはloadVersionで判定
	constexpr int32 loadVersion = 0;
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
				Print << U"Cannot read 'GameData.bin!'";
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
