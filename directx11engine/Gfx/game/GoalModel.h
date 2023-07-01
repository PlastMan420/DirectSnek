#pragma once
#include <SimpleMath.h>
#include "../abstract/BaseModel.h"

using namespace DirectX::SimpleMath;
using dxdRect = DirectX::SimpleMath::Rectangle;

class GoalModel: public BaseModel
{
public:
    void Draw(ID2D1DeviceContext1* d2dContext);
    void GenFood(D2D1_POINT_2F coordinates);

    D2D1_POINT_2F currentCoordinates;

private:

};

