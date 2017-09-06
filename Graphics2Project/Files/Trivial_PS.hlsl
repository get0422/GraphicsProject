
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

struct Lighting
{
	float4 direction;
	float4 color;
	float4 position;
	float4 radius;
};

cbuffer Lighting : register(b0)
{
	Lighting lights[3];
}



float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	float4 baseColor = tex.Sample(samp,input.uvH);

	// Ambient Light
	float4 ambient = float4(0.25, 0.25, 0.25, 1);

	// Directional light
	float directionalLightRA = saturate(dot(normalize(-lights[0].direction), normalize(input.normalH)));
	float4 directionalresult = directionalLightRA * lights[0].color * baseColor;

	// Point Light // 
	//float4 pointLightDirection = normalize(lights[1].position - input.worldpos);
	//float pointLightratio = saturate(dot(pointLightDirection, input.normalH));
	//float pointLightAttenuation = 1.0 - saturate(length(lights[1].position - input.worldpos) / 10);
	//float4 pointLightResult = pointLightratio * lights[1].color * baseColor * pointLightAttenuation;



	baseColor = saturate(ambient + directionalresult);

	return (baseColor * tex.Sample(samp, input.uvH));
}
