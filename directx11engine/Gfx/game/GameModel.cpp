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

void GameModel::Init(ID2D1DeviceContext1* d2dcontext)
{
	arena.Init(d2dcontext);
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
}

void GameModel::Update(ID2D1DeviceContext1* d2dcontext, ID2D1SolidColorBrush* sBrush)
{
	//draw arena
	arena.Draw(d2dcontext);
	//draw snek
	snek.Draw(d2dcontext, sBrush);

	if (!weGotDinner)
	{
		D2D1_POINT_2F foodGenCo = GenRange();
		food.GenFood(foodGenCo);

		weGotDinner = true;
	}

	food.Draw(d2dcontext);

	if (food.Intersects(snek))
	{
		weGotDinner = false;
		snek.Grow();
		snek.SpeedUp();
		score += 1;
	}

	//check for collision
	if (!arena.Intersects(snek)) {
		Reset();
	}
}

void GameModel::UpdateDirection(int dirEnum)
{
	snek.UpdateDirection(dirEnum);
}
