#pragma once
#include <SimpleMath.h>
#include "../abstract/BaseModel.h"

using namespace DirectX::SimpleMath;
using dxdRect = DirectX::SimpleMath::Rectangle;

class GoalModel: public BaseModel
{
public:
    void Draw(ID2D1HwndRenderTarget* renderTarget);
    void GenFood(D2D1_POINT_2F coordinates);

    D2D1_POINT_2F currentCoordinates;
};

