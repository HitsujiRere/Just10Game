
# pragma once
# include "Common.hpp"
# include "Cell.hpp"

// �^�C�g���V�[��
class HowTo : public MyApp::Scene
{
private:

	// ����
	String desc;

	// �g�p����摜
	Array<Texture> descImages =
	{
	};

public:

	HowTo(const InitData& init);

	void update() override;

	void draw() const override;
};
