#include"../Adapter/AdapterReader.h"
#include "Shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "Camera.h"
#include "../Timer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Model.h";
#include <d2d1.h>
#include <dwrite.h>
#include <windowsnumerics.h>
#include <queue>
#include <wincodec.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <cstdlib>
#include <ctime>
#include <directxcollision.h>

using namespace DirectX::SimpleMath;

class Gfx {
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	void ClearScreen(float r, float g, float b);
	void DrawArena();
	void BeginDraw();
	void EndDraw();
	void Reset();
	void UpdateDirection(int dirEnum);
	void RunDemo();

	Camera camera;
	Model model;
	Gfx();
	~Gfx();

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();
	bool InitDeviceAndAdapter(HWND hwnd);
	bool InitDepthStencilAndBlenderState();
	void InitImGui(HWND hwnd);
	void InitSolidBrush();
	void UseBrickTextureBrush();
	void ShutdownImGui();
	void CountFps();
	void DrawSnek();
	void GenRange();
	void CollisionDetection();
	//ID2D1SolidColorBrush* GetASolidBrush(float r, float g, float b, float a = 1);

	int fpsCounter;
	int lastFps;
	int xpos;
	int ypos;
	bool positionsX;
	bool positionsY;
	int direction;
	const int step = 50;

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
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pinkTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pavementTexture;

	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	ConstantBuffer<CB_PS_pixelshader> cb_ps_pixelshader;

	VertexShader vertexShader;
	PixelShader pixelShader;

	//Microsoft::WRL::ComPtr<ID2D1Factory> d2dfactory;
	//Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> renderTarget;
	//Microsoft::WRL::ComPtr<IDWriteFactory> dwFactory;
	//Microsoft::WRL::ComPtr<IDWriteTextFormat> dwTextFormat;
	//Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> sBrush;
	//Microsoft::WRL::ComPtr<ID2D1PathGeometry> sBrush;

	ID2D1Factory* d2dfactory;
	ID2D1HwndRenderTarget* renderTarget;
	IDWriteFactory* dwFactory;
	IDWriteTextFormat* dwTextFormat;
	ID2D1SolidColorBrush* sBrush;
	ID2D1BitmapBrush* bmBrush;
	ID2D1Bitmap* arenaBM;
	//ID2D1PathGeometry* snekGeometry;

	std::queue<D2D1_POINT_2F> snekPath;

	//they are unique pointers cause they will reclaim themselves after exiting the scope
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