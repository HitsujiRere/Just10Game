
# include "Battle.hpp"

Battle::Battle(const InitData& init)
	: IScene(init)
{
}

void Battle::update()
{
	if (false)
	{
		changeScene(State::Title);
		getData().highScore = Max(getData().highScore, m_score);
	}
}

void Battle::draw() const
{
}
