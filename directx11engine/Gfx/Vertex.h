// Vertex structure.
#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex(){}

	//2D coordinates since we are doing 2D rendering for now. for 3D rendering, add a third float type for third dimension.
	Vertex(float x, float y, float depth, float u, float v) : pos(x, y, depth), texCoord(u, v) {}

	// 3 floats for xy coordinates and a third float for stencil depth (3D.... kinda)
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
};

