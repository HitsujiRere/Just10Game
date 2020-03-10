
# pragma once
# include "Common.hpp"
# include "Cell.hpp"

// タイトルシーン
class HowTo : public MyApp::Scene
{
private:

	// 説明
	String desc;
	//Array<String> descs = Array<String>();

public:

	HowTo(const InitData& init);

	void update() override;

	void draw() const override;
};
