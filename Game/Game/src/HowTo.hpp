
# pragma once
# include "Common.hpp"

// タイトルシーン
class HowTo : public MyApp::Scene
{
private:

public:

	HowTo(const InitData& init);

	void update() override;

	void draw() const override;
};
