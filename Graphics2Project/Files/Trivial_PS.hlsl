
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
	float4 worldpos : WORLD_POSITION;
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
	Lighting Dlight;
}



float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	// Gets Texture Color
	float4 TextureColor = tex.Sample(samp,input.uvH);

	// Ambient Light
	float4 ambient = float4(0.25, 0.25, 0.25, 1);

	// Directional light
	float directionalLightRA = saturate(dot(normalize(-Dlight.direction), normalize(input.normalH)));
	float4 directionalresult = directionalLightRA * Dlight.color * TextureColor;

	// Point Light // 
	//float4 pointLightDirection = normalize(lights[1].position - input.worldpos);
	//float pointLightratio = saturate(dot(pointLightDirection, input.normalH));
	//float pointLightAttenuation = 1.0 - saturate(length(lights[1].position - input.worldpos) / 10);
	//float4 pointLightResult = pointLightratio * lights[1].color * TextureColor * pointLightAttenuation;


	//+ pointLightResult 
	TextureColor = saturate(ambient + directionalresult);

	return (TextureColor * tex.Sample(samp, input.uvH));
}
