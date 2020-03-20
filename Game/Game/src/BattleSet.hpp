
# pragma once

# include "Common.hpp"

// バトル前設定
class BattleSet : public MyApp::Scene
{
private:

public:

	BattleSet(const InitData& init);

	void update() override;

	void draw() const override;
};
