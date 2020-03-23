
# pragma once

# include "Common.hpp"
# include "Battle.hpp"
# include "Character.hpp"

// バトル前設定
class BattleSet : public MyApp::Scene
{
private:

	// キャラクター選択のスライダー
	Array<int32> charactersChoiseNum;

	// キャラクター選択を確定したかどうか
	Array<bool> charactersChoiseEnd;

public:

	BattleSet(const InitData& init);

	void update() override;

	void draw() const override;
};
