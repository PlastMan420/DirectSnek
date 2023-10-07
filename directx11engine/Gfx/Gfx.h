#include"../Adapter/AdapterReader.h"
#include "Shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Camera.h"
#include "../Helpers/Timer.h"
#include <dwrite.h>
#include <windowsnumerics.h>
#include <SimpleMath.h>
#include "./game/GameModel.h";
#include <d2d1_2.h>
#include <d2d1_1helper.h>

class Gfx {
public:
	Gfx();
	~Gfx();
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	void ClearScreen(float r, float g, float b);
	void BeginDraw();
	void EndDraw();
	void InitGame();
	void DrawTray();

	Camera camera;
	GameModel game;

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();
	bool InitDevice(HWND hwnd);
	bool InitDepthStencilAndBlenderState();
	void InitSolidBrush();
	void CountFps();
	bool InitEffects();
	bool InitSwapChain(HWND hwnd);
	bool CreateD2DContext();

	int fpsCounter;
	int lastFps;

	//DXGI COM Pointers
	// D3D11 Device
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11DeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Device1> d2dDevice;
	Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;

	// SwapChain
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	Microsoft::WRL::ComPtr<IDXGISurface> dxgiBackBuffer;

	// Rasterizer state
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;

	// Blender state
	Microsoft::WRL::ComPtr<ID3D11BlendState> blenderState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;

	//D2D COM Pointers
	Microsoft::WRL::ComPtr<ID2D1Factory2> d2dfactory;
	//Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> renderTarget;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1> d2dContext;

	// Brushes
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> sBrush;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2dTargetBitmap;

	// Direct Write
	Microsoft::WRL::ComPtr<IDWriteFactory> dwFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> dwTextFormat;

	// Effects
	//Microsoft::WRL::ComPtr<ID2D1Effect> gaussianBlurEffect;

	// Textures
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pinkTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pavementTexture;

	//ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	//ConstantBuffer<CB_PS_pixelshader> cb_ps_pixelshader;

	VertexShader vertexShader;
	PixelShader pixelShader;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	int windowWidth = 0;
	int windowHeight = 0;
	float aspectRatio = 0.0f;
	float nearZ = 0.1f;
	float farZ = 1000.0f;

	Timer fpsTimer;
	Timer clock;

	// This array defines the set of DirectX hardware feature levels this app  supports.
	// The ordering is important and you should  preserve it.
	// Don't forget to declare your app's minimum required feature level in its
	// description.
	D3D_FEATURE_LEVEL featureLevels[3] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
	};
};
