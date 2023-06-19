#include"../Adapter/AdapterReader.h"
#include "Shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Camera.h"
#include "../Helpers/Timer.h"
#include <d2d1.h>
#include <dwrite.h>
#include <windowsnumerics.h>
#include <SimpleMath.h>
#include "./game/GameModel.h";

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

	Camera camera;
	GameModel game;

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();
	bool InitDeviceAndAdapter(HWND hwnd);
	bool InitDepthStencilAndBlenderState();
	void InitSolidBrush();
	void CountFps();

	int fpsCounter;
	int lastFps;

	//DXGI COM Pointers
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blenderState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;

	// Textures
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pinkTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pavementTexture;

	//ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	//ConstantBuffer<CB_PS_pixelshader> cb_ps_pixelshader;

	//D2D COM Pointers
	Microsoft::WRL::ComPtr<ID2D1Factory> d2dfactory;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> renderTarget;
	Microsoft::WRL::ComPtr<IDWriteFactory> dwFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> dwTextFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> sBrush;

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
};
