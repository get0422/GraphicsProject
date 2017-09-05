
// Non-user Defined
Texture2D tex : register(t0);
SamplerState samp : register(s0);

// User Defined Stuff from Vertex Needed for the Pixel Shader
struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float4 colorOut : COLOR0;
	float2 uvH : UV0;
	float4 normalH : NORMAL0;
};

cbuffer ConstantMatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}

// Pixel Shader
float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	//return input.colorOut;
	return tex.Sample(samp, input.uvH);
}

