
// Non-user Defined
TextureCube skyboxTexture : register(t0);
SamplerState sampl : register(s0);


// User Defined Stuff from Vertex Needed for the Pixel Shader
struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float4 colorOut : COLOR0;
	float3 uvH : UV0;
	float4 normalH : NORMAL0;
	float4 worldpos : WORLD_POSITION;
};

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	// Gets Texture Color
	float4 TextureColor = skyboxTexture.Sample(sampl,input.uvH);
	return TextureColor;
}
