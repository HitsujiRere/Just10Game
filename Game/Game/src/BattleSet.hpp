
# pragma once

# include "Common.hpp"
# include "Character.hpp"

// バトル前設定
class BattleSet : public MyApp::Scene
{
private:

	// キャラクター選択のスライダー
	double charactersSlider = 0.0;

public:

	BattleSet(const InitData& init);

	void update() override;

	void draw() const override;
};
