#pragma once
#include <wincodec.h>
#include <WICTextureLoader.h>
#include "../../Helpers/ErrorLogger.h"
#include <SimpleMath.h>
#include "../abstract/BaseModel.h"
#include <wrl/client.h>

using namespace DirectX::SimpleMath;
using dxdRect = DirectX::SimpleMath::Rectangle;

class ArenaModel: BaseModel
{
public:
    void Draw(ID2D1HwndRenderTarget* renderTarget);
    void clean();
    bool Intersects(dxdRect* rect);
    void Init(ID2D1HwndRenderTarget* renderTarget);

private:
    //ID2D1BitmapBrush* bmBrush;
    //ID2D1Bitmap* arenaBM;

    Microsoft::WRL::ComPtr<ID2D1BitmapBrush> bmBrush;
    Microsoft::WRL::ComPtr<ID2D1Bitmap> arenaBM;

    void UseBrickTextureBrush(ID2D1HwndRenderTarget* _renderTarget);
};

