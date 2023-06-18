#include "ArenaModel.h"

void ArenaModel::clean()
{
	if (this->bmBrush) {
		bmBrush->Release();
	}

	if (this->arenaBM) {
		arenaBM->Release();
	}
}

void ArenaModel::Draw(ID2D1HwndRenderTarget* renderTarget)
{
	try {
		HRESULT hr;

		if (!initRan)
		{
			hr = E_ABORT;
			COM_ERROR_IF_FAILED(hr, "Init() neglected. Please run the Init() of your class.");
		}

		//HRESULT hr;
		D2D1_SIZE_F rtSize = renderTarget->GetSize();

		D2D1_RECT_F rect1 = D2D1::RectF(50, 100.0f, rtSize.width - 50, rtSize.height - 100.0f);

		//this->renderTarget->BeginDraw();
		renderTarget->DrawRectangle(rect1, bmBrush.Get(), 32.0f);
		//COM_ERROR_IF_FAILED(hr, "Failed to draw arena");
		//this->renderTarget->EndDraw();
	}
	catch (COMException& ex)
	{
		ErrorLogger::Log(ex);
	}
}

void ArenaModel::UseBrickTextureBrush(ID2D1HwndRenderTarget* renderTarget)
{
	try
	{
		HRESULT hr;
		IWICImagingFactory* wicF = NULL;
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)&wicF
		);

		IWICBitmapDecoder* wicDecoder;
		hr = wicF->CreateDecoderFromFilename(
			L"Data\\Textures\\whitebrick.png",
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&wicDecoder
		);
		COM_ERROR_IF_FAILED(hr, "Failed to load bitmap brick.png");

		//read frame from image. we want the whole image, which is a single frame. so index = 0;
		IWICBitmapFrameDecode* wicFrame = NULL;
		hr = wicDecoder->GetFrame(0, &wicFrame);
		COM_ERROR_IF_FAILED(hr, "Failed to load bitmap brick.png. get frame error.");

		IWICFormatConverter* wicConv = NULL;
		wicF->CreateFormatConverter(&wicConv);

		wicConv->Initialize(
			wicFrame,
			GUID_WICPixelFormat32bppPBGRA, //pixel format
			WICBitmapDitherTypeNone, //no dithering
			NULL, //Palette, not needed
			0.0, // alpha transparency, not needed
			WICBitmapPaletteTypeCustom
		);

		renderTarget->CreateBitmapFromWicBitmap(
			wicConv,
			NULL, //D2D BITMAP PROPERTIES
			arenaBM.GetAddressOf()
		);


		hr = renderTarget->CreateBitmapBrush(
			arenaBM.Get(),
			&bmBrush
		);

		bmBrush->SetExtendModeX(D2D1_EXTEND_MODE_WRAP);
		bmBrush->SetExtendModeY(D2D1_EXTEND_MODE_WRAP);

		wicConv->Release();
		wicF->Release();
		wicDecoder->Release();
		wicFrame->Release();
	}
	catch (COMException& ex)
	{
		ErrorLogger::Log(ex);
	}
}

bool ArenaModel::Intersects(BaseModel model)
{
	dxdRect arenaBoxTop(50, 0, 750, 1); //x: 50 -> 700, y: 100 -> 116
	dxdRect arenaBoxBottom(50, 500, 750, 1); // x: 50 -> 700, y: 450: 466

	dxdRect arenaBoxRight(800, 120, 1, 600); // x: 750 -> 166, y: 120 -> 720
	dxdRect arenaBoxLeft(10, 120, 1, 600); // x: 50 -> 66, y: 120: 720

	if (model.hitBox.Intersects(arenaBoxTop)) {
		return true;
	}
	else if (model.hitBox.Intersects(arenaBoxBottom)) {
		return true;
	}
	else if (model.hitBox.Intersects(arenaBoxRight)) {
		return true;
	}
	else if (model.hitBox.Intersects(arenaBoxLeft)) {
		return true;
	}

	return false;
}

void ArenaModel::Init(ID2D1HwndRenderTarget* renderTarget)
{
	UseBrickTextureBrush(renderTarget);

	initRan = true;
}
