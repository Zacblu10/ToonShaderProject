
TextureCube skyboxTexture	:register(t0);
SamplerState samplerOptions	:register(s0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 samplerDir	: DIRECTION;
};


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
	//sample the skybox
	return skyboxTexture.Sample(samplerOptions, input.samplerDir); 
}

