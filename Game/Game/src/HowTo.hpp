
# pragma once
# include "Common.hpp"
# include "Cell.hpp"

// タイトルシーン
class HowTo : public MyApp::Scene
{
private:

	// タイトル
	const String headerText = U"How To Play";

	// 説明
	String desc;

	Rect backButton;
	Transition backTransition = Transition(0.4s, 0.2s);

public:

	HowTo(const InitData& init);

	void update() override;

	void draw() const override;
};
