//
//TextureCube skyboxTexture : register(t0);
//
//SamplerState sampl : register(s0);
//
//
//struct OUTPUT_VERTEX
//{
//	float4 projectedCoordinate : SV_POSITION;
//	float4 colorOut : COLOR0;
//	float2 uvH : UV0;
//	float4 normalH : NORMAL0;
//};
//
//float4 main(OUTPUT_VERTEX input) : SV_TARGET
//{
//	return skyboxTexture.Sample(sampl, input.colorOut.xyz);
//}

TextureCube skyboxTexture : register(t0);

SamplerState filter : register(s0);


struct OUTPUT_VERTEX
{
	float4 posH : SV_POSITION;
	float3 textH : TEXTURECOORD;
};

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	return skyboxTexture.Sample(filter, input.textH.xyz);
}