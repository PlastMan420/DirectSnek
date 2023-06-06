#include "gfx.h"

Gfx::Gfx()
{
	this->d2dfactory = NULL;
	this->renderTarget = NULL;
}

Gfx::~Gfx()
{
	// Release all your COM interfaces. do not leak memory you big baka.
	if (this->d2dfactory) {
		d2dfactory->Release();
	}

	if (this->renderTarget) {
		renderTarget->Release();
	}
}

bool Gfx::Initialize(HWND hwnd, int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	this->aspectRatio = static_cast<float>(this->windowWidth) / static_cast<float>(this->windowHeight);
	this->fpsTimer.Start();
	if(!InitializeDirectX(hwnd))
		return false;

	if (!InitializeShaders())
		return false;
	
	if (!InitializeScene())
		return false;

	//setup imgui

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

void Gfx::RenderFrame()
{
	float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgColor);
	//after clearing render view, clear depth stencil view.
	// 1.0f is max depth.
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// important init before drawing. drawing without these might crash GPU.
	this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());

	// draw a triangle. check other options
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->deviceContext->RSSetState(this->rasterizerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	
	//for transparency
	//this->deviceContext->OMSetBlendState(this->blenderState.Get(), NULL, 0xFFFFFFFF);
	this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);

	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);

	UINT offset = 0;
	//static float alpha = 1.0f;
	{
		this->model.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	}

	//Update Constant Buffers
	//move square by (0.0, 0.5). it will move upwards.
	//DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	static float translationOffset[3] = { 0, 0, 0 };
	DirectX::XMMATRIX world = XMMatrixScaling(5.0f, 5.0f, 5.0f) * DirectX::XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);

	cb_vs_vertexshader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();

	// transpose.
	cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);
	
	if (!cb_vs_vertexshader.ApplyChanges())
		return;

	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());
	
	// Draw text
	static int fpsCounter = 0;
	static std::string fpsString = "D3D11: 0";
	fpsCounter += 1;

	if (fpsTimer.GetMilisecondsElapsed() > 1000.0) 
	{
		fpsString = "D3D11: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}

	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();

	//start imgui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//create imgui test window
	ImGui::Begin("test");
	ImGui::Text("sample text");

	ImGui::DragFloat3("Translation", translationOffset, 0.1f, -5.0f, 5.0f);

	ImGui::End();

	//assemble things
	ImGui::Render();

	//draw
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	this->swapChain->Present(1, NULL);
}

bool Gfx::InitializeDirectX(HWND hwnd)
{
	try {

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

		HRESULT hr;

		// INIT DEVICE

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


		spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

		//Create sampler description for sampler state
		CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // for x coordinate
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // for y coordinate
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // for 3D textures

		hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

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

	if (!vertexShader.Initialize(this->device, shaderFolder + L"vertexshader.cso", layout, numElements)) {
		return false;
	}

	if (!pixelShader.Initialize(this->device, shaderFolder + L"PixelShader.cso"))
		return false;

	return true;
}

bool Gfx::InitializeScene()
{
	try {
		HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_grass.jpg", nullptr, grassTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_pavement.jpg", nullptr, pavementTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\pinksquare.jpg", nullptr, pinkTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = this->cb_ps_pixelshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		if (!model.Initialize(this->device.Get(), this->deviceContext.Get(), this->pavementTexture.Get(), cb_vs_vertexshader))
		{
			return false;
		}
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
