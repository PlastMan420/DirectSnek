//OG:
//The "type" does not matter, as long as it matches.
// ex: float3 inTexCoord: poopoo; where "poopoo" is a valid "type".
// but it has to be used where a type of "poopoo" is expected.
//-----------------------------------------------------------------------------
//10/06/2023: https://lettier.github.io/3d-game-shaders-for-beginners/texturing.html

//cbuffer alphaBuffer : register(b0)
//{
//	float alpha;
//}

//struct PS_INPUT {
//	float4 inPos: SV_POSITION;
//	float2 texCoord: TEXCOORD;
//};

//Texture2D objTexture: TEXTURE : register(t0);
//SamplerState objSamplerState: SAMPLER : register(s0);

//float4 main(PS_INPUT input) : SV_TARGET
//{
//	float3 pixelColor = objTexture.Sample(objSamplerState, input.texCoord);
//	return float4(pixelColor, alpha);
//}
cbuffer CBuf : register(b0)
{
    float4 color;
    float2 dimensions;
    float radius;
    float na;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    uint viewId : SV_RenderTargetArrayIndex;
};

bool ShouldDiscard(float2 coords, float2 dimensions, float radius)
{
    float2 circle_center = float2(radius, radius);

    if (length(coords - circle_center) > radius
        && coords.x < circle_center.x && coords.y < circle_center.y)
        return true; //first circle
        
    circle_center.x += dimensions.x - 2 * radius;
        
    if (length(coords - circle_center) > radius
        && coords.x > circle_center.x && coords.y < circle_center.y)
        return true; //second circle
        
    circle_center.y += dimensions.y - 2 * radius;
    
    if (length(coords - circle_center) > radius
        && coords.x > circle_center.x && coords.y > circle_center.y)
        return true; //third circle
        
    circle_center.x -= dimensions.x - 2 * radius;
        
    if (length(coords - circle_center) > radius
        && coords.x < circle_center.x && coords.y > circle_center.y)
        return true; //fourth circle
        
    return false;
        
}

float4 main(VSOut input) : SV_TARGET
{
    float2 coords = input.uv * dimensions;
    if (ShouldDiscard(coords, dimensions, radius))
        discard;
    return color;
}
