//The "type" does not matter, as long as it matches.
// ex: float3 inTexCoord: poopoo; where "poopoo" is a valid "type".
// but it has to be used where a type of "poopoo" is expected.

cbuffer alphaBuffer : register(b0)
{
	float alpha;
}

struct PS_INPUT {
	float4 inPos: SV_POSITION;
	float2 texCoord: TEXCOORD;
};

Texture2D objTexture: TEXTURE : register(t0);
SamplerState objSamplerState: SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 pixelColor = objTexture.Sample(objSamplerState, input.texCoord);
	return float4(pixelColor, alpha);
}