#pragma once
#include "../abstract/BaseModel.h"
#include <queue>
#include "../../Helpers/ErrorLogger.h"
#include <d2d1_2.h>

class SnekModel : public BaseModel
{
public:
    void Draw(ID2D1DeviceContext1* d2dContext, ID2D1SolidColorBrush* sBrush);
	void UpdateDirection(int dirEnum);
	void Reset();
	void Init();
	int Grow();
	void SpeedUp();

private:
	void GenerateSnek();

	bool initRan = false;
    std::queue<D2D1_POINT_2F> snekPath;
	float xpos;
	float ypos;
	bool positionsX;
	bool positionsY;
	int direction;
	const int snakeBlockDistance = 50;
	float step = 0.4f;
};

