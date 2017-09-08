
struct INPUT_GEOMETRY
{
	float4 coordinate : SV_POSITION;
	float4 colorOut : COLOR0;
	float2 uvH : UV0;
	float4 normalH : NORMAL0;
	float4 worldpos : WORLD_POSITION;
};

cbuffer ConstantMatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}

[maxvertexcount(4)]
void main(point float4 input[1] : SV_POSITION, inout TriangleStream<INPUT_GEOMETRY> triStream)
{
	INPUT_GEOMETRY geo[4];

	geo[0].coordinate = float4(-7, 0, 5, 1);
	geo[0].uvH = float2(1, 1);
	geo[0].normalH = float4(0, 0, -1, 0);
	geo[0].worldpos.xyz = geo[3].coordinate.xyz;

	geo[1].coordinate = float4(-7, 8, 5, 1);
	geo[1].uvH = float2(1, 0);
	geo[1].normalH = float4(0, 0, -1, 0);
	geo[1].worldpos.xyz = geo[1].coordinate.xyz;
	
	geo[2].coordinate = float4(1, 0, 5, 1);
	geo[2].uvH = float2(0, 1);
	geo[2].normalH = float4(0, 0, -1, 0);
	geo[2].worldpos.xyz = geo[2].coordinate.xyz;

	geo[3].coordinate = float4(1, 8, 5, 1);
	geo[3].uvH = float2(0, 0);
	geo[3].normalH = float4(0, 0, -1, 0);
	geo[3].worldpos = float4(input[0].xyz, 1);

	for (unsigned int i = 0; i < 4; i++) {
		float4 localH = geo[i].coordinate;

		localH = mul(localH, WorldMatrix);
		localH = mul(localH, ViewMatrix);
		localH = mul(localH, ProjectionMatrix);

		geo[i].coordinate = localH;
		triStream.Append(geo[i]);
	}
}