#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
#include<vector>
#include<wrl/client.h>
#include <d3d11.h>
#include"../Helpers/ErrorLogger.h"

class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC description;
};

class AdapterReader {
public:
	static std::vector<AdapterData> GetAdapters();

private:
	static std::vector<AdapterData> adapters;
};

