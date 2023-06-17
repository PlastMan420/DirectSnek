#pragma once
#include <d2d1.h>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
using dxdRect = DirectX::SimpleMath::Rectangle;

class BaseModel
{
    public:
    virtual void Draw();
};

