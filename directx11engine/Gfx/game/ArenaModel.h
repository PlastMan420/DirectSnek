#pragma once
#include <wincodec.h>
#include <WICTextureLoader.h>
#include "../../Helpers/ErrorLogger.h"
#include "../abstract/BaseModel.h"

class ArenaModel: public BaseModel
{
public:
    void Draw(ID2D1DeviceContext1* d2dContext);
    bool Intersects(BaseModel model);
    void Init(ID2D1DeviceContext1* d2dContext);

private:
    bool initRan = false;

    Microsoft::WRL::ComPtr<ID2D1BitmapBrush> bmBrush;
    Microsoft::WRL::ComPtr<ID2D1Bitmap> arenaBM;

    void UseBrickTextureBrush(ID2D1DeviceContext1* d2dContext);
};
