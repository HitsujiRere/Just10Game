
# pragma once
# include "Common.hpp"
# include "Cell.hpp"

// �^�C�g���V�[��
class HowTo : public MyApp::Scene
{
private:

	// �^�C�g��
	const String headerText = U"How To Play";

	// ����
	String desc;

	Rect backButton;
	Transition backTransition = Transition(0.4s, 0.2s);

public:

	HowTo(const InitData& init);

	void update() override;

	void draw() const override;
};
