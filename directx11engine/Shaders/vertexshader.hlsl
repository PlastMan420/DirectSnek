//OG:  to declate a constant buffer, you need to register to a buffer slot bx (b0 in this case)
//-------------------------------------------------------------------------------------------------------
//10/06/2023: https://lettier.github.io/3d-game-shaders-for-beginners/texturing.html

cbuffer cBuffer : register(b0)
{
	//all are column_major by default, since its more efficient, DirectX Math library however uses row_major.
	//instead of switching to row_major, you transpose the matrix in the code before setting constantBuffer.
    float4x4 mat;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.outPos = mul(float4(input.inPos, 1.0f), mat);
    output.texCoord = input.texCoord;
    return output;
}
