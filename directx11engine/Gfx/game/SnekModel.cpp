#include "SnekModel.h"

void SnekModel::Init()
{
	this->xpos = 0;
	this->ypos = 0;
	positionsX = true;
	positionsY = false;
	direction = 4; //D
	initRan = true;
}

int SnekModel::Grow()
{
	D2D1_POINT_2F head = D2D1::Point2F(100 + this->xpos, 150 + this->ypos);

	snekPath.push(head);

	return snekPath.size();
}

void SnekModel::Draw(ID2D1DeviceContext1* d2dContext, ID2D1SolidColorBrush* sBrush)
{
	try {
		HRESULT hr;
		
		if (!initRan)
		{
			hr = E_ABORT;
			COM_ERROR_IF_FAILED(hr, "Init() neglected. Please run the Init() of your class.");
		}

		if (snekPath.empty())
		{
			GenerateSnek();
		}

		this->hitBox = dxdRect(150 + this->xpos, 150 + this->ypos, 50, 50);

		if (xpos < 0 || xpos > 600) Reset();
		if (ypos < 0 || ypos > 600) Reset();

		//remove tails. add new head in front. length delta is 0.

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

		D2D1_POINT_2F trailer = snekPath.front();
		snekPath.pop();
		D2D1_POINT_2F trails = snekPath.front();
		snekPath.pop();

		std::queue<D2D1_POINT_2F> snekPathCache;

		while (!snekPath.empty())
		{
			d2dContext->DrawLine(
				trailer,
				trails,
				sBrush,
				2.5f
			);

			trailer = trails;
			snekPathCache.push(trailer);
			trails = snekPath.front();
			snekPath.pop();
		}

		d2dContext->DrawLine(
			trailer,
			trails,
			sBrush,
			2.5f
		);

		D2D1_POINT_2F head = D2D1::Point2F(100 + this->xpos, 150 + this->ypos);

		d2dContext->DrawLine(
			trails,
			head,
			sBrush,
			2.5f
		);

		snekPathCache.push(trails); //new head
		snekPathCache.push(head); //new head

		snekPath = snekPathCache;
	}
	catch (COMException& ex)
	{
		ErrorLogger::Log(ex);
	}
}

void SnekModel::UpdateDirection(int dirEnum)
{
	if (dirEnum == 0)
	{
		return;
	}

	if (dirEnum > 2 && (direction == dirEnum - 2))
	{
		return;
	}
	else if (dirEnum < 3 && (direction == dirEnum + 2))
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

	GenerateSnek();
}

void SnekModel::GenerateSnek()
{
	if(!snekPath.empty())
	{
		return;
	}

	int lengthAllowance = 600;
	while (lengthAllowance > 200)
	{
		snekPath.push(D2D1::Point2F(lengthAllowance +this->xpos, 150 + this->ypos));
		lengthAllowance -= 50;
	}
}

void SnekModel::SpeedUp()
{
	if (step >= 2.0f)
	{
		return;
	}

	step += 0.02f;
}