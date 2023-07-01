#include "ArenaModel.h"

void ArenaModel::Draw(ID2D1DeviceContext1* d2dContext)
{
	try {
		HRESULT hr;

		if (!initRan)
		{
			hr = E_ABORT;
			COM_ERROR_IF_FAILED(hr, "Init() neglected. Please run the Init() of your class.");
		}

		//HRESULT hr;
		D2D1_SIZE_F rtSize = d2dContext->GetSize();

		D2D1_RECT_F rect1 = D2D1::RectF(80, 130.0f, rtSize.width - 80, rtSize.height - 100.0f);

		//this->d2dContext->BeginDraw();
		d2dContext->DrawRectangle(rect1, bmBrush.Get(), 32.0f);
		//COM_ERROR_IF_FAILED(hr, "Failed to draw arena");
		//this->d2dContext->EndDraw();
	}
	catch (COMException& ex)
	{
		ErrorLogger::Log(ex);
	}
}

void ArenaModel::UseBrickTextureBrush(ID2D1DeviceContext1* d2dContext)
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

		d2dContext->CreateBitmapFromWicBitmap(
			wicConv,
			NULL, //D2D BITMAP PROPERTIES
			arenaBM.GetAddressOf()
		);


		hr = d2dContext->CreateBitmapBrush(
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
	dxdRect arenaBoxTop(50, 0, 750, 0); //x: 50 -> 700, y: 100 -> 116
	dxdRect arenaBoxBottom(50, 530, 750, 0); // x: 50 -> 700, y: 450: 466

	dxdRect arenaBoxRight(800, 120, 0, 600); // x: 750 -> 166, y: 120 -> 720
	dxdRect arenaBoxLeft(50, 120, 0, 600); // x: 50 -> 66, y: 120: 720

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

void ArenaModel::Init(ID2D1DeviceContext1* d2dContext)
{
	UseBrickTextureBrush(d2dContext);

	initRan = true;
}
