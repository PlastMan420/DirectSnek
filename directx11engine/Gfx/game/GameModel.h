#pragma once
#include "ArenaModel.h"
#include "SnekModel.h"
#include "GoalModel.h"
#include "../../Helpers/ErrorLogger.h"
#include <random>

class GameModel
{
public:
	void Reset();
	void RunDemo();
	void Clean();
	void Update(ID2D1HwndRenderTarget* _renderTarget, ID2D1SolidColorBrush* _sBrush);
	void UpdateDirection(int dirEnum);
	void Init(ID2D1HwndRenderTarget* _renderTarget);

	ArenaModel arena;
	SnekModel snek;
	GoalModel food;
	int score = 0;

private:
	D2D1_POINT_2F GenRange();
	bool weGotDinner = false;

	// random device and number generator
	std::random_device rd;
};
