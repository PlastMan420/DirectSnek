#pragma once
#include <d2d1.h>
#include <SimpleMath.h>
#include <wrl/client.h>

using namespace DirectX::SimpleMath;
using dxdRect = DirectX::SimpleMath::Rectangle;

class BaseModel
{
public:
    virtual void Draw(ID2D1HwndRenderTarget* renderTarget)
    {

    }
    virtual bool Intersects(BaseModel model)
    {
        return model.hitBox.Intersects(hitBox);
    }

    dxdRect hitBox;
};
