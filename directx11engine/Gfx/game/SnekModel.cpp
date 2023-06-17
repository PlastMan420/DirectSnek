#include "SnekModel.h"

SnekModel::SnekModel()
{
}

void SnekModel::Init()
{
	this->xpos = 0;
	this->ypos = 0;
	positionsX = true;
	positionsY = false;
	direction = 4; //D

}

void SnekModel::Draw(ID2D1HwndRenderTarget* _renderTarget, ID2D1SolidColorBrush* _sBrush)
{
	if (snekPath.size() == 0)
	{
		snekPath.push(D2D1::Point2F(350 + this->xpos, 150 + this->ypos)); //tail first in first out
		snekPath.push(D2D1::Point2F(300 + this->xpos, 150 + this->ypos)); //body
		snekPath.push(D2D1::Point2F(250 + this->xpos, 150 + this->ypos)); //body2
		snekPath.push(D2D1::Point2F(200 + this->xpos, 150 + this->ypos)); //head
	}

	snekBox = dxdRect(150 + this->xpos, 150 + this->ypos, 50, 50);

	if (xpos < 0 || xpos > 600) Reset();
	if (ypos < 0 || ypos > 600) Reset();

	//fifo. pop removes tail. push adds head
	D2D1_POINT_2F tail = snekPath.front();
	snekPath.pop();

	D2D1_POINT_2F body = snekPath.front();
	snekPath.pop();

	D2D1_POINT_2F body2 = snekPath.front();
	snekPath.pop();

	D2D1_POINT_2F head = snekPath.front();
	snekPath.pop();

	_renderTarget->DrawLine(
		head,
		body2,
		_sBrush,
		2.5f
	);

	_renderTarget->DrawLine(
		body2,
		body,
		_sBrush,
		2.5f
	);

	_renderTarget->DrawLine(
		body,
		tail,
		_sBrush,
		2.5f
	);

	switch (direction)
	{
	case 1:
		this->ypos -= step;
		break;
	case 2:
		xpos -= step;
		break;
	case 3:
		ypos += step;
		break;
	case 4:
		xpos += step;
		break;
	default:
		break;
	}

	//fifo
	snekPath.push(body); //new tail is old body
	snekPath.push(body2); //new tail is old body
	snekPath.push(head); //new body is old head
	snekPath.push(D2D1::Point2F(100 + this->xpos, 150 + this->ypos)); //new head
}

void SnekModel::UpdateDirection(int dirEnum)
{
	if (dirEnum == 0)
	{
		return;
	}

	if (dirEnum > 2 && (dirEnum - 2 == direction))
	{
		return;
	}

	direction = dirEnum;
}

void SnekModel::Reset()
{
	xpos = 0;
	ypos = 0;
	direction = 4;

	while (!snekPath.empty())
	{
		snekPath.pop();
	}

	snekPath.push(D2D1::Point2F(350 + this->xpos, 150 + this->ypos)); //tail first in first out
	snekPath.push(D2D1::Point2F(300 + this->xpos, 150 + this->ypos)); //body
	snekPath.push(D2D1::Point2F(250 + this->xpos, 150 + this->ypos)); //body2
	snekPath.push(D2D1::Point2F(200 + this->xpos, 150 + this->ypos)); //head
}