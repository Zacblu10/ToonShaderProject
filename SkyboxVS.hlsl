
cbuffer EnternalData : register(b0)
{
	matrix view;
	matrix projection;
}

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

struct VertexToPixel
{
	float4 position		: SV_POSITION; 
	float3 samplerDir	: DIRECTION; 
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixel main( VertexShaderInput input )
{
	// Set up output struct
	VertexToPixel output;

	//copy view matrix and set the translation to 0 
	matrix viewNoTranslation = view; 
	viewNoTranslation._14 = 0; 
	viewNoTranslation._24 = 0;
	viewNoTranslation._34 = 0;

	//Apply projection and updated view to the input position and save it to the output. 
	matrix wvp = mul(projection, viewNoTranslation);
	output.position = mul(wvp, float4(input.position, 1.0f));

	//ensure output depth is 1
	output.position.z = output.position.w; 

	//calculate sampler direction
	output.samplerDir = input.position;

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}