#pragma once
#include <wincodec.h>
#include <WICTextureLoader.h>
#include "../../Helpers/ErrorLogger.h"
#include "../abstract/BaseModel.h"

class ArenaModel: public BaseModel
{
public:
    void Draw(ID2D1HwndRenderTarget* renderTarget);
    void clean();
    bool Intersects(BaseModel model);
    void Init(ID2D1HwndRenderTarget* renderTarget);

private:
    bool initRan = false;

    Microsoft::WRL::ComPtr<ID2D1BitmapBrush> bmBrush;
    Microsoft::WRL::ComPtr<ID2D1Bitmap> arenaBM;

    void UseBrickTextureBrush(ID2D1HwndRenderTarget* _renderTarget);
};
