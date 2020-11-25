#include "ShaderUtils.hlsli"

Texture2D diffuseTexture	:register(t0); 
SamplerState samplerOptions	:register(s0);


cbuffer EnternalData : register(b0)
{
	Light light1;
	Light light2;
	Light light3;
	Light light4;
	float specularValue; 
	float3 cameraPosition;

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
float4 main(VertexToPixel input) : SV_TARGET
{
	//grab texture data 
	float3 surfaceColor = pow(diffuseTexture.Sample(samplerOptions, input.uv).rgb, 2.2f);

float3 dirLightColor =
	GetLightColor(input.normal, light1, surfaceColor, input.color, input.worldPos, specularValue, cameraPosition) +
	GetLightColor(input.normal, light2, surfaceColor, input.color, input.worldPos, specularValue, cameraPosition) +
	GetLightColor(input.normal, light3, surfaceColor, input.color, input.worldPos, specularValue, cameraPosition) +
	GetLightColor(input.normal, light4, surfaceColor, input.color, input.worldPos, specularValue, cameraPosition);

	//return the color
	return float4(pow(dirLightColor, 1.0f / 2.2f), 1);
}

