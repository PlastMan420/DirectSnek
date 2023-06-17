#pragma once
#include <DirectXMath.h>

struct CB_VS_vertexshader {
	DirectX::XMMATRIX mat;
};

struct CB_PS_pixelshader {
    DirectX::XMFLOAT4 color;
    DirectX::XMFLOAT2 dimensions;
    float radius;
    float na;
};
