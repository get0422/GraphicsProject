struct INPUT_VERTEX
{
	float3 coordinate : POSITION;
	float4 color : COLOR;
	float2 uvL : UV;
	float4 normalL : NORMAL;
};

struct OUTPUT_VERTEX
{
	float4 posH : POSITION;
	float2 uvH : UV;
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
	float4 normalH : NORMAL;
};

cbuffer THIS_IS_VRAM : register( b0 )
{
	float4 constantColor;
	float2 constantOffset;
	float2 padding;
};

cbuffer OBJECT : register(b0)
{
	float4x4 WorldMatrix;
}

cbuffer SCENE : register(b1)
{
	float4x4 ViewMatrix;
	float4x4 ProjectionMatrix;
}



OUTPUT_VERTEX main( INPUT_VERTEX input)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;
	// ensures translation is preserved during matrix multiply
	float4 localH = float4(input.coordinate.xyz, 1);
	// move local space vertex from vertex buffer into world space.
	localH = mul(localH, WorldMatrix);

	output.projectedCoordinate = localH;
	// TODO: Move into view space, then projection space
	localH = mul(localH, ViewMatrix);
	localH = mul(localH, ProjectionMatrix);

	output.posH = localH;
	output.uvH = input.uvL;
	output.normalH = input.normalL;

	return output;
}

