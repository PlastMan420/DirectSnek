#include "GameModel.h"

void GameModel::RunDemo()
{

	//if (snsnekBox.Intersects(arenaBoxTop)) {
	//	positionsX = true;
	//	positionsY = false;
	//}

	//else if (snekBox.Intersects(arenaBoxBottom)) {
	//	positionsX = true;
	//	positionsY = false;
	//}
	//else if (snekBox.Intersects(arenaBoxRight)) {
	//	positionsX = false;
	//	positionsY = true;
	//}
	//else if (snekBox.Intersects(arenaBoxLeft2)) {
	//	positionsX = false;
	//	positionsY = true;
	//}
	//else if (snekBox.Intersects(arenaBoxLeft1)) {
	//	positionsX = false;
	//	positionsY = true;
	//}

	//DrawSnek();

	//if (positionsX && this->ypos == 0) {
	//	// goes right --->
	//	this->xpos += step;
	//}
	////then go down
	//else if (positionsY && this->xpos > 0) {
	//	// vvv
	//	this->ypos += step;
	//}
	//// then go left
	//else if (positionsX && this->xpos > 0) {
	//	this->xpos -= step;
	//}
	//// then go up
	//else {
	//	this->ypos -= step;
	//}
	/////////////////////////////////////////////////////////////////////////
	//go right
//if (this->xpos <= xposStep && this->ypos == 0) {
//	// goes right --->
//	this->xpos += step;
//}
////then go down
//else if (this->xpos >= xposStep && this->ypos < 300) {
//	// vvv
//	this->ypos += step;
//}
//// then go left
//else if (this->ypos >= 300 && this->xpos > 0) {
//	// goes botton vvv
//	this->xpos -= step;
//}
//// then go up
//else {
//	this->ypos -= step;
//}

}

void GameModel::Init(ID2D1HwndRenderTarget* _renderTarget)
{
	arena.Init(_renderTarget);
	snek.Init();
}

void GameModel::Reset()
{
	snek.Reset();
	this->score = 0;
}

D2D1_POINT_2F GameModel::GenRange()
{
	std::uniform_int_distribution<int> xp(100, 560);
	std::uniform_int_distribution<int> yp(150, 320);

	return D2D1::Point2F(xp(rd), yp(rd));
}
void GameModel::Clean()
{
	arena.clean();
}

void GameModel::Update(ID2D1HwndRenderTarget* _renderTarget, ID2D1SolidColorBrush* _sBrush)
{
	//draw arena
	arena.Draw(_renderTarget);

	if (!weGotDinner)
	{
		D2D1_POINT_2F foodGenCo = GenRange();
		food.GenFood(foodGenCo);

		weGotDinner = true;
	}

	food.Draw(_renderTarget);

	if (food.Intersects(snek))
	{
		weGotDinner = false;
		snek.Grow();
		score += 1;
	}

	//check for collision
	if (arena.Intersects(snek)) {
		Reset();
	}

	//draw snek
	snek.Draw(_renderTarget, _sBrush);
}

void GameModel::UpdateDirection(int dirEnum)
{
	snek.UpdateDirection(dirEnum);
}
