#pragma pack_matrix(row_major)

struct INPUT_VERTEX
{
	float4 posL : POSITION;
};

struct OUTPUT_VERTEX
{
	float4 posH : SV_POSITION;
	float3 textH : TEXTURECOORD;
};

cbuffer ConstantMatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}

OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;
	float4 localH = float4(input.posL.xyz, 1);

	localH = mul(localH, WorldMatrix);
	output.textH = localH.xyz;

	localH = mul(localH, ViewMatrix);
	localH = mul(localH, ProjectionMatrix);

	output.posH = localH;
	
	return output;
}


//#pragma pack_matrix(row_major)
//
//struct INPUT_VERTEX
//{
//	float4 coordinate : POSITION;
//	float4 color : COLOR;
//	float2 uvL : UV;
//	float4 normalL : NORMAL;
//};
//
//struct OUTPUT_VERTEX
//{
//	float4 projectedCoordinate : SV_POSITION;
//	float4 colorOut : COLOR0;
//	float2 uvH : UV0;
//	float4 normalH : NORMAL0;
//};
//
//cbuffer ConstantMatrixBuffer : register(b0)
//{
//	matrix WorldMatrix;
//	matrix ViewMatrix;
//	matrix ProjectionMatrix;
//}
//
//OUTPUT_VERTEX main(INPUT_VERTEX input)
//{
//	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;
//	float4 localH = float4(input.coordinate.xyz, 1);
//	localH = mul(localH, WorldMatrix);
//	output.colorOut = float4(localH.xyz, 1);
//
//	localH = mul(localH, ViewMatrix);
//	localH = mul(localH, ProjectionMatrix);
//
//	output.projectedCoordinate = localH;
//
//	return output;
//}
