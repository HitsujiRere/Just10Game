
# pragma once

# include "Common.hpp"
# include "Cell.hpp"

// タイトルシーン
class Title : public MyApp::Scene
{
private:

	Rect placticeButton = Rect(Arg::center = Scene::Center().movedBy(0, 0), 300, 60);
	Transition placticeTransition = Transition(0.4s, 0.2s);

	Rect battle2PButton = Rect(Arg::center = Scene::Center().movedBy(0, 100), 300, 60);
	Transition battle2PTransition = Transition(0.4s, 0.2s);

	Rect battleCPUButton = Rect(Arg::center = Scene::Center().movedBy(0, 200), 300, 60);
	Transition battleCPUTransition = Transition(0.4s, 0.2s);

	Rect howtoButton = Rect(Arg::center = Scene::Center().movedBy(0, 300), 300, 60);
	Transition howtoTransition = Transition(0.4s, 0.2s);

	Rect exitButton = Rect(Arg::center = Scene::Center().movedBy(0, 400), 300, 60);
	Transition exitTransition = Transition(0.4s, 0.2s);

public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;
};
