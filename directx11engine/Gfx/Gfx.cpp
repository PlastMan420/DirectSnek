#include "gfx.h"

Gfx::Gfx()
{
	this->d2dfactory = NULL;
	this->renderTarget = NULL;
	this->fpsCounter = 0;
	this->lastFps = 0;
	this->xpos = 0;
	this->ypos = 0;
	positionsX = true;
	positionsY = false;
	direction = 4; //D
}

Gfx::~Gfx()
{
	// Release all COM
	if (this->d2dfactory) {
		d2dfactory->Release();
	}

	if (this->renderTarget) {
		renderTarget->Release();
	}

	if (this->dwFactory) {
		dwFactory->Release();
	}

	if (this->dwTextFormat) {
		dwTextFormat->Release();
	}

	if (this->sBrush) {
		sBrush->Release();
	}

	if (this->bmBrush) {
		bmBrush->Release();
	}
	
	if (this->arenaBM) {
		arenaBM->Release();
	}
}

bool Gfx::Initialize(HWND hwnd, int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	this->aspectRatio = static_cast<float>(this->windowWidth) / static_cast<float>(this->windowHeight);
	this->fpsTimer.Start();
	this->clock.Start();
	if(!InitializeDirectX(hwnd))
		return false;

	//if (!InitializeShaders())
	//	return false;
	//
	//if (!InitializeScene())
	//	return false;

	return true;
}

void Gfx::InitImGui(HWND hwnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();
}

void Gfx::ShutdownImGui()
{
	// Shutdown
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Gfx::RenderFrame()
{
	//run init device and init stencil and init shaders for the commented code below.
	// 
	// important init before drawing. drawing without these might crash GPU.
	//this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());

	// draw a triangle. check other options
	//this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//this->deviceContext->RSSetState(this->rasterizerState.Get());
	//this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);

	//for transparency
	//this->deviceContext->OMSetBlendState(this->blenderState.Get(), NULL, 0xFFFFFFFF);
	//this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);

	//this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	//this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	//this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);


	//start imgui frame
	//ImGui_ImplDX11_NewFrame();
	//ImGui_ImplWin32_NewFrame();
	//ImGui::NewFrame();

	double ms = clock.GetMilisecondsElapsed();

	// Update FPS
	if (ms < 100.0)
	{
		return;
	}

	BeginDraw();
	ClearScreen(0.0f, 0.0f, 0.0f);
	CountFps();
	/*spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();*/

	DrawArena();
	DrawSnek();

	EndDraw();

	clock.Restart();
	//assemble things
	//ImGui::Render();

	//draw
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//this->swapChain->Present(1, NULL);
}

void Gfx::CountFps()
{
	D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();

	this->fpsCounter += 1;

	double ms = fpsTimer.GetMilisecondsElapsed();

	// Update FPS
	if (ms > 1000.0)
	{
		this->lastFps = this->fpsCounter;
		std::wstring txt = L"D2D: " + std::to_wstring(this->fpsCounter);
		
		this->renderTarget->DrawTextW(
			txt.c_str(),
			txt.length(),
			this->dwTextFormat,
			D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
			sBrush
		);

		this->fpsCounter = 0;
		fpsTimer.Restart();
	}
	else
	{
		std::wstring txt = L"D2D: " + std::to_wstring(this->lastFps);

		this->renderTarget->DrawTextW(
			txt.c_str(),
			txt.length(),
			this->dwTextFormat,
			D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
			sBrush
		);
	}
}

bool Gfx::InitDeviceAndAdapter(HWND hwnd)
{
	HRESULT hr;

	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1) {
		ErrorLogger::Log("No DXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// INIT DISPLAY
	scd.BufferDesc.Height = this->windowHeight;
	scd.BufferDesc.Width = this->windowWidth;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//important //
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	///

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// INIT DEVICE AND SWAPCHAIN

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

	hr = D3D11CreateDeviceAndSwapChain(
		adapters[0].pAdapter, // reference to adapter
		D3D_DRIVER_TYPE_UNKNOWN, // driver type, unknown means unspecified
		NULL, //software driver type
		NULL, // runtime layers
		NULL, //D3D feature level
		0, // number of feature levels
		D3D11_SDK_VERSION, // version, use sdk version for direct xtk
		&scd, // swapchain description
		this->swapChain.GetAddressOf(), // swapchain address
		this->device.GetAddressOf(), // device address
		NULL, // supported feature level
		this->deviceContext.GetAddressOf() // device context address.
	);
	COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	COM_ERROR_IF_FAILED(hr, "GetBuffer Failed");

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");
}

bool Gfx::InitDepthStencilAndBlenderState()
{

	if (!this->device) {
		throw COMException(
		NULL,
		"Cannot init depth stencil and blender state without initializing device and device context. please run InitDeviceAndAdapter() first",
		__FILE__,
		__FUNCTION__,
		__LINE__);
	}

	HRESULT hr;

	// INIT DEPTH STENCIL
		// Use CD3D11_TEXTURE2D_DESC instead of D3D11_TEXTURE2D_DESC for less config to write.
		//D3D11_TEXTURE2D_DESC depthStencilDesc; OLD
	CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, this->windowWidth, this->windowHeight);
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

	hr = this->device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

	// output merger.
	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

	//Create depth stencil state.
	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	//Create depth stencil state
	CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
	depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

	//Create and Set the Viewport
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(this->windowWidth), static_cast<float>(this->windowHeight));

	// if you want split screen like in older games, you can add more viewports.
	this->deviceContext->RSSetViewports(1, &viewport);

	//Create Rasterizer State
	CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
	hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

	//Create blend state
	D3D11_BLEND_DESC blendDesc = { 0 };
	D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0] = rtbd;

	hr = this->device->CreateBlendState(&blendDesc, this->blenderState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create blender state.");
}

bool Gfx::InitializeDirectX(HWND hwnd)
{
	try {
		HRESULT hr;

		//InitDeviceAndAdapter(hwnd);
		//InitDepthStencilAndBlenderState();

		//spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		//spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

		////Create sampler description for sampler state
		//CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
		//sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // for x coordinate
		//sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // for y coordinate
		//sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // for 3D textures

		//hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
		//COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");


		////init sprites
		//spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		//spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

		// INIT D2D
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dfactory);
		COM_ERROR_IF_FAILED(hr, "Failed init D2D Factory: Create Factory failed");

		RECT rect;
		GetClientRect(hwnd, &rect);
		hr = d2dfactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rect.right, rect.bottom)),
			&renderTarget
		);
		COM_ERROR_IF_FAILED(hr, "Failed init D2D Factory: Create Render Target Failed.");

		// Init main brush
		InitSolidBrush();
		UseBrickTextureBrush();

		// INIT DWrite
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(dwFactory),
			reinterpret_cast<IUnknown**>(&dwFactory)
		);
		COM_ERROR_IF_FAILED(hr, "Failed init DWrite.");

		hr = dwFactory->CreateTextFormat(
			L"Verdana",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			14,
			L"", //locale
			&dwTextFormat
		);
		COM_ERROR_IF_FAILED(hr, "Failed init DWrite: CreateTextFormat Error.");

		dwTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		dwTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

		return true;
	}
	catch (COMException& ex)
	{
		ErrorLogger::Log(ex);
		return false;
	}
}

bool Gfx::InitializeShaders()
{
	std::wstring shaderFolder;

#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE) {
	#ifdef _DEBUG
		#ifdef _WIN64
			shaderFolder = L"..\\x64\\Debug\\";
		#else
			shaderFolder = L"..\\x64\\Debug\\"
		#endif
	#else
		#ifdef _WIN64
			shaderFolder = L"..\\x64\\Release\\";
		#else
			shaderFolder = L"..\\\\Release\\";
		#endif //  _WIN64
	#endif
	}
#pragma endregion


	D3D11_INPUT_ELEMENT_DESC layout[] = {
		//for 3D use R32G32B32 instead of R32G32.
		//use D3D11_APPEND_ALIGNED_ELEMENT instead of the fifth parameter later for offsets.
		//D3D11_INPUT_PER_VERTEX_DATA and the 0 in sixth and 7th parameters mean no instancing.
		// the "POSITION" name is in vertexshader.hlsl
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layout);

	//if (!vertexShader.Initialize(this->device, shaderFolder + L"vertexshader.cso", layout, numElements)) {
	//	return false;
	//}

	//if (!pixelShader.Initialize(this->device, shaderFolder + L"PixelShader.cso"))
	//	return false;

	return true;
}

bool Gfx::InitializeScene()
{
	try {
		//HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_grass.jpg", nullptr, grassTexture.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_pavement.jpg", nullptr, pavementTexture.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\pinksquare.jpg", nullptr, pinkTexture.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		HRESULT hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = this->cb_ps_pixelshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		/*if (!model.Initialize(this->device.Get(), this->deviceContext.Get(), this->pavementTexture.Get(), cb_vs_vertexshader))
		{
			return false;
		}*/
	}
	catch (COMException& ex)
	{
		ErrorLogger::Log(ex);
		return false;
	}

	camera.SetPosition(0.0f, 0.0f, -2.0f);
	camera.SetProjectionValues(90.0f, this->aspectRatio, this->nearZ, this->farZ);

	return true;
}

void Gfx::ClearScreen(float r, float g, float b) {
	this->renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Gfx::DrawArena()
{
	try {
		D2D1_SIZE_F rtSize = renderTarget->GetSize();

		D2D1_RECT_F rect1 = D2D1::RectF(50, 100.0f, rtSize.width - 50, rtSize.height - 100.0f);

		renderTarget->DrawRectangle(rect1, bmBrush, 32.0f);

		D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();
		this->renderTarget->DrawTextW(
			L"(50.0f, 70.0f)",
			14,
			this->dwTextFormat,
			D2D1::RectF(50, 70.0f, renderTargetSize.width, renderTargetSize.height),
			sBrush
		);

		std::wstring widthtxt = std::to_wstring(static_cast<int>(rtSize.width - 50));
		std::wstring heighttxt = std::to_wstring(static_cast<int>(rtSize.height - 100));
		std::wstring txt = L"(" + widthtxt + L", " + heighttxt + L")";

		this->renderTarget->DrawTextW(
			txt.c_str(),
			txt.length(),
			this->dwTextFormat,
			D2D1::RectF(rtSize.width - 120, rtSize.height - 80.0f, renderTargetSize.width, renderTargetSize.height),
			sBrush
		);
	}
	catch (COMException& ex)
	{
		ErrorLogger::Log(ex);
	}
}

void Gfx::DrawSnek()
{
	// window:
		//width = 800px -> 50 : 750
		//height = 600px -> 100 : 500

	if (snekPath.size() == 0)
	{
		snekPath.push(D2D1::Point2F(350 + this->xpos, 150 + this->ypos)); //tail first in first out
		snekPath.push(D2D1::Point2F(300 + this->xpos, 150 + this->ypos)); //body
		snekPath.push(D2D1::Point2F(250 + this->xpos, 150 + this->ypos)); //body2
		snekPath.push(D2D1::Point2F(200 + this->xpos, 150 + this->ypos)); //head
	}

	// x, y, w, h
	SimpleMath::Rectangle arenaBoxTop(50, 100, 750, 16); //x: 50 -> 700, y: 100 -> 116
	SimpleMath::Rectangle arenaBoxBottom(50, 450, 750, 16); // x: 50 -> 700, y: 450: 466

	SimpleMath::Rectangle arenaBoxRight(750, 120, 16, 600); // x: 750 -> 166, y: 120 -> 720
	SimpleMath::Rectangle arenaBoxLeft1(50, 120, 16, 300); // x: 50 -> 66, y: 120: 720
	SimpleMath::Rectangle arenaBoxLeft2(50, 350, 16, 300); // x: 50 -> 66, y: 120: 720

	//BoundingBox arenaBox;

	SimpleMath::Rectangle snekBox(150 + this->xpos, 150 + this->ypos, 50, 50);

	if (snekBox.Intersects(arenaBoxTop)) {
		Reset();
	}
	
	else if (snekBox.Intersects(arenaBoxBottom)) {
		Reset();
	}
	else if (snekBox.Intersects(arenaBoxRight)) {
		Reset();
	}
	else if (snekBox.Intersects(arenaBoxLeft2)) {
		Reset();
	}
	else if (snekBox.Intersects(arenaBoxLeft1)) {
		Reset();
	}
	if (xpos < 0 || xpos > 600) Reset();
	if (ypos < 0 || ypos > 600) Reset();

	//fifo. pop removes tail. push adds head
	D2D1_POINT_2F tail = snekPath.front();
	snekPath.pop();

	D2D1_POINT_2F body = snekPath.front();
	snekPath.pop();

	D2D1_POINT_2F body2 = snekPath.front();
	snekPath.pop();

	D2D1_POINT_2F head = snekPath.front();
	snekPath.pop();

	renderTarget->DrawLine(
		head,
		body2,
		sBrush,
		2.5f
	);

	renderTarget->DrawLine(
		body2,
		body,
		sBrush,
		2.5f
	);

	renderTarget->DrawLine(
		body,
		tail,
		sBrush,
		2.5f
	);

	switch (direction)
	{
	case 1:
		this->ypos -= step;
		break;
	case 2:
		xpos -= step;
		break;
	case 3:
		ypos += step;
		break;
	case 4:
		xpos += step;
		break;
	default:
		break;
	}

	//fifo
	snekPath.push(body); //new tail is old body
	snekPath.push(body2); //new tail is old body
	snekPath.push(head); //new body is old head
	snekPath.push(D2D1::Point2F(100 + this->xpos, 150 + this->ypos)); //new head
}

void Gfx::RunDemo()
{
	SimpleMath::Rectangle arenaBoxTop(50, 100, 750, 16); //x: 50 -> 700, y: 100 -> 116
	SimpleMath::Rectangle arenaBoxBottom(50, 450, 750, 16); // x: 50 -> 700, y: 450: 466

	SimpleMath::Rectangle arenaBoxRight(750, 120, 16, 600); // x: 750 -> 166, y: 120 -> 720
	SimpleMath::Rectangle arenaBoxLeft1(50, 120, 16, 300); // x: 50 -> 66, y: 120: 720
	SimpleMath::Rectangle arenaBoxLeft2(50, 350, 16, 300); // x: 50 -> 66, y: 120: 720

	//BoundingBox arenaBox;

	SimpleMath::Rectangle snekBox(150 + this->xpos, 150 + this->ypos, 50, 50);

	if (snekBox.Intersects(arenaBoxTop)) {
		positionsX = true;
		positionsY = false;
	}

	else if (snekBox.Intersects(arenaBoxBottom)) {
		positionsX = true;
		positionsY = false;
	}
	else if (snekBox.Intersects(arenaBoxRight)) {
		positionsX = false;
		positionsY = true;
	}
	else if (snekBox.Intersects(arenaBoxLeft2)) {
		positionsX = false;
		positionsY = true;
	}
	else if (snekBox.Intersects(arenaBoxLeft1)) {
		positionsX = false;
		positionsY = true;
	}
	BeginDraw();
	DrawArena();
	DrawSnek();
	if (positionsX && this->ypos == 0) {
		// goes right --->
		this->xpos += step;
	}
	//then go down
	else if (positionsY && this->xpos > 0) {
		// vvv
		this->ypos += step;
	}
	// then go left
	else if (positionsX && this->xpos > 0) {
		this->xpos -= step;
	}
	// then go up
	else {
		this->ypos -= step;
	}

	//go right
//if (this->xpos <= xposStep && this->ypos == 0) {
//	// goes right --->
//	this->xpos += step;
//}
////then go down
//else if (this->xpos >= xposStep && this->ypos < 300) {
//	// vvv
//	this->ypos += step;
//}
//// then go left
//else if (this->ypos >= 300 && this->xpos > 0) {
//	// goes botton vvv
//	this->xpos -= step;
//}
//// then go up
//else {
//	this->ypos -= step;
//}

	EndDraw();
}

void Gfx::Reset()
{
	xpos = 0;
	ypos = 0;
	direction = 4;

	while (!snekPath.empty())
	{
		snekPath.pop();
	}

	snekPath.push(D2D1::Point2F(350 + this->xpos, 150 + this->ypos)); //tail first in first out
	snekPath.push(D2D1::Point2F(300 + this->xpos, 150 + this->ypos)); //body
	snekPath.push(D2D1::Point2F(250 + this->xpos, 150 + this->ypos)); //body2
	snekPath.push(D2D1::Point2F(200 + this->xpos, 150 + this->ypos)); //head
}

void Gfx::UpdateDirection(int dirEnum)
{
	if (dirEnum == 0)
	{
		return;
	}
	direction = dirEnum;
}

void Gfx::CollisionDetection()
{
	if (positionsX && !positionsY)
	{
		positionsY = true;
		positionsX = false;
	}
	else if (!positionsX && positionsY)
	{
		positionsY = false;
		positionsX = true;
	}
}

void Gfx::InitSolidBrush()
{
	HRESULT hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(255.0f, 255.0f, 255.0f, 1), &sBrush);
	COM_ERROR_IF_FAILED(hr, "Failed to initialize brush");
}

void Gfx::UseBrickTextureBrush()
{
	try
	{
		IWICImagingFactory* wicF = NULL;
		HRESULT hr = CoCreateInstance(
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
			&arenaBM
		);


		hr = renderTarget->CreateBitmapBrush(
			arenaBM,
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

//ID2D1SolidColorBrush* Gfx::GetASolidBrush(float r = 255.0f, float g = 255.0f, float b = 255.0f, float a = 1.0f)
//{
//	ID2D1SolidColorBrush* brush;
//
//	HRESULT hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
//	COM_ERROR_IF_FAILED(hr, "Failed to initialize brush");
//
//	return brush;
//}

void Gfx::BeginDraw()
{
	renderTarget->BeginDraw();
}

void Gfx::EndDraw()
{
	renderTarget->EndDraw();
}

//rtSize.width / 2 - 100.0f,
//rtSize.height / 2 - 100.0f,
//rtSize.width / 2 + 100.0f,
//rtSize.height / 2 + 100.0f
void Gfx::GenRange()
{
	srand(time(0));
	int xco = rand() % 301 + 300;
	int yco = rand() % 301 + 300;

	D2D1::Point2F(xco, 150 + this->ypos);
}