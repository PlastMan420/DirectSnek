#include "GoalModel.h"

void GoalModel::Draw(ID2D1HwndRenderTarget* renderTarget)
{
	const int radius = 5;

	ID2D1SolidColorBrush* sBrush;
	HRESULT hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(255.0f, 255.0f, 255.0f, 1), &sBrush);

	D2D1_RECT_F rect1 = D2D1::RectF(currentCoordinates.x, currentCoordinates.y, currentCoordinates.x + radius, currentCoordinates.y + radius);
	hitBox = dxdRect(currentCoordinates.x, currentCoordinates.y, radius, radius);

	renderTarget->FillRectangle(rect1, sBrush);
}

void GoalModel::GenFood(D2D1_POINT_2F coordinates)
{
	currentCoordinates = coordinates;
}