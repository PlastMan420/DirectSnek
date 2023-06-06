#pragma once
#include "../Vertex.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../ConstantBuffer.h"
#include "../Model.h"
#include <SimpleMath.h>

using namespace DirectX;

class arena : public Model
{
public:

private:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const XMMATRIX& viewProjectionMatrix);
	//XMMATRIX worldMatrix = XMMatrixIdentity();

};
