
# pragma once
# include "Common.hpp"

// ゲームシーン
class Battle : public MyApp::Scene
{
private:
	// スコア
	int32 m_score = 0;

public:

	Battle(const InitData& init);

	void update() override;

	void draw() const override;
};
