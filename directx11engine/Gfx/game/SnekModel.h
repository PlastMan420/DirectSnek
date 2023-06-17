#pragma once
#include "../abstract/BaseModel.h"
#include <queue>
#include <wrl/client.h>

class SnekModel : BaseModel
{
public:
    SnekModel();
    void Draw(ID2D1HwndRenderTarget* _renderTarget, ID2D1SolidColorBrush* _sBrush);
	void UpdateDirection(int dirEnum);
	void Reset();
	void Init();

	int xpos;
	int ypos;
	bool positionsX;
	bool positionsY;
	int direction;
	const int step = 50;

	dxdRect snekBox;

private:
    std::queue<D2D1_POINT_2F> snekPath;
	//Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> _sBrush;

};

