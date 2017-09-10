struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
	float4 color : COLOR;
	float3 uvL : UV;
	float4 normalL : NORMAL;
};

struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float4 colorOut : COLOR0;
	float3 uvH : UV0;
	float4 normalH : NORMAL0;
	float4 worldpos : WORLD_POSITION;
};

cbuffer ConstantMatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Variable for the vertex input and output , gonna be the world position and your gonna get the position after you multipley the pistion of the vertex by the model matrix
OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;

	float4 localH = float4(input.coordinate.xyz, 1);

	localH = mul(localH, WorldMatrix);
	output.uvH = localH.xyz;

	localH = mul(localH, ViewMatrix);
	localH = mul(localH, ProjectionMatrix);

	output.projectedCoordinate = localH;

	return output;
}

