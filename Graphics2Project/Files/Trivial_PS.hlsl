
// Non-user Defined
Texture2D tex : register(t0);
SamplerState samp : register(s0);

// User Defined Stuff from Vertex Needed for the Pixel Shader
struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float4 colorOut : COLOR0;
	float3 uvH : UV0;
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
	Lighting lights[3];
}

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	// Gets Texture Color
	float4 TextureColor = tex.Sample(samp,input.uvH);

	// Ambient Light
	float4 ambient = float4(0.25, 0.25, 0.25, 1);

	// Spot Light
	float4 spotLightDirection = normalize(lights[0].position - input.worldpos);
	spotLightDirection.w = 0;

	float surfaceRatio = saturate(dot(-spotLightDirection, lights[0].direction));
	float spotfactor = (surfaceRatio > lights[0].radius.x) ? 1 : 0;
	float spotLightRatio = saturate(dot(spotLightDirection, input.normalH));

	float spotLightAttenuation = 1.0f - saturate(length(lights[0].position - input.worldpos) / lights[0].radius.z);
	float spotLightCone = 1.0f - saturate((lights[0].radius.x - surfaceRatio) / (lights[0].radius.x - lights[0].radius.y));

	float4 spotLightResult = spotfactor * spotLightRatio * lights[0].color * TextureColor * spotLightAttenuation * spotLightCone;

	// Point Light 
	float  pointLightratio = saturate(dot(normalize(lights[1].position - input.worldpos), input.normalH));
	float  pointLightAttenuation = 1.0 - saturate(length(lights[1].position - input.worldpos) / lights[1].radius);
	float4 pointLightResult = pointLightratio * lights[1].color * pointLightAttenuation * (pointLightAttenuation * pointLightAttenuation) * TextureColor;

	// Directional light
	float directionalLightRA = saturate(dot(normalize(-lights[2].direction), normalize(input.normalH)));
	float4 directionalresult = directionalLightRA * lights[2].color * TextureColor;

	 
	TextureColor = saturate(ambient + spotLightResult + pointLightResult + directionalresult) * TextureColor;


	return TextureColor;
}
