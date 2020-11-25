#ifndef _SHADER_UTILITIES_
#define _SHADER_UTILITIES_

// - Structs ===========================================================================
//struct for  light
struct Light
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 dirOrPos;
	int type;
	float intensity; 
};

struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;       // Normal Vector
	float2 uv			: UV;			//UV Texture coords
	float3 tangent		: TANGENT;
};

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float3 worldPos		: W_POSITION;
	float2 uv			: TEXCOORD;
};

struct V2P_NormalMap
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float3 worldPos		: W_POSITION;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT; 
};

// - Helper Functions =======================================================================

float3 GetLightColor(float3 normal, Light light, float3 surfaceColor, float4 colorTint, float3 worldPos, float specularValue, float3 cameraPosition)
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
	float3 specular = 0;
	if (specularValue == 0)
	{
		specular = 0;
	}
	else
	{
		//get the view vector
		float3 V = normalize(cameraPosition - worldPos);

		//get the reflection vector
		float3 R = reflect(dir, normal);

		//calculate specular
		specular = pow(saturate(dot(R, V)), specularValue);
	}


	//calculate light amount 
	float3 lightAmount = saturate(dot(normal, -dir));

	float3 diffuse = lightAmount * light.diffuseColor * surfaceColor;
	float3 ambient = light.ambientColor * surfaceColor * (float3)colorTint;

	//if there is no diffuse, there is no specular
	specular *= any(diffuse); 
	
	return diffuse + ambient + specular;
}
#endif