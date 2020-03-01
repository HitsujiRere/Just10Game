
# include "HowTo.hpp"

HowTo::HowTo(const InitData& init)
	: IScene(init)
{

}

void HowTo::update()
{
}

void HowTo::draw() const
{
	const String headerText = U"‚ ‚»‚Ñ‚©‚½";
	const Vec2 center(Scene::Center().x, 40);
	FontAsset(U"Header")(headerText).drawAt(center.movedBy(4, 6), ColorF(0.0, 0.5));
	FontAsset(U"Header")(headerText).drawAt(center);

	Rect(0, 500, Scene::Width(), Scene::Height() - 500)
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));
}
