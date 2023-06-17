#pragma once
#include "ArenaModel.h"
#include "SnekModel.h"
#include <cstdlib>
#include <ctime>

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

private:
	void GenRange();

};

