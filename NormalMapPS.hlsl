#include "PBRUtils.hlsli"
#include "ShaderUtils.hlsli"


Texture2D Albedo			:register(t0);
Texture2D NormalMap			:register(t1);
Texture2D RoughnessMap		:register(t2); 
Texture2D MetalnessMap		:register(t3); 
SamplerState SamplerOptions	:register(s0);


cbuffer EnternalData : register(b0)
{
	Light light1;
	Light light2;
	float3 cameraPosition;

}

float3 GetPBRLightColor(float3 normal, Light light, float3 albedoColor, float4 colorTint, float3 worldPos, float3 cameraPosition, float roughness, float metalness)
{
	float3 dir = 0;
	if (light.type == 0)
	{
		//negate light direction and normalize 
		dir = -light.dirOrPos;
		dir = normalize(dir);
	}
	else if (light.type == 1)
	{
		dir = light.dirOrPos - worldPos;
		dir = normalize(dir);
	}

	//Specular=======================================
	//get the view vector
	float3 V = normalize(cameraPosition - worldPos);

	// Specular color determination -----------------
	// Assume albedo texture is actually holding specular color where metalness == 1
	//
	// Note the use of lerp here - metal is generally 0 or 1, but might be in between
	// because of linear texture sampling, so we want lerp the specular color to match 
	float3 specularColor = lerp(F0_NON_METAL.rrr, albedoColor.rgb, metalness);

	//calculate light amount 
	float diff = DiffusePBR(normal, dir);
	float3 spec = MicrofacetBRDF(normal, dir, V, roughness, metalness, specularColor);

	//calculate diifuse with energy conservation
	float3 balancedDiff = DiffuseEnergyConserve(diff, spec, metalness); 

	//combine final diffuse and specular for this light
	float3 total = (balancedDiff * albedoColor + spec) * light.ambientColor * light.intensity;

	return total;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(V2P_NormalMap input) : SV_TARGET
{
	//grab texture data 
	float3 albedoColor = pow(Albedo.Sample(SamplerOptions, input.uv).rgb, 2.2f); 

	//grab normal map data
	float3 unpackedNormal = NormalMap.Sample(SamplerOptions, input.uv).rgb * 2 - 1; 

	//grab the roughness map data
	float roughness = RoughnessMap.Sample(SamplerOptions, input.uv).r; 

	//grab the metalness map data
	float metalness = MetalnessMap.Sample(SamplerOptions, input.uv).r; 

	//ensure normal and tangent are normalized. 
	float3 N = normalize(input.normal); 
	float3 T = normalize(input.tangent); 
	//Gram-schmidt 
	T = normalize(T - N * dot(T, N)); 
	//calculate bitangent
	float3 B = cross(T, N); 
	float3x3 TBN = float3x3(T, B, N);

	//Transform the unpacked normal
	input.normal = normalize(mul(unpackedNormal, TBN));

	float3 dirLightColor =
		GetPBRLightColor(input.normal, light1, albedoColor, input.color, input.worldPos, cameraPosition, roughness, metalness) + 
		GetPBRLightColor(input.normal, light2, albedoColor, input.color, input.worldPos, cameraPosition, roughness, metalness);
	

	//return the color
	return float4(pow(dirLightColor, 1.0f/2.2f),1);
}

