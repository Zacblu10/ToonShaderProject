#include "Sky.h"

Sky::Sky(Mesh* skyMesh, Microsoft::WRL::ComPtr<ID3D11SamplerState> _samplerOptions, Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context, std::wstring textureFilepath, std::wstring vsFilepath, std::wstring psFilepath)
{
	skyGeometry = skyMesh; 
	samplerOptions = _samplerOptions; 

	//load shaders
	vertexShader = new SimpleVertexShader(_device.Get(), _context.Get(), vsFilepath.c_str()); 
	pixelShader = new SimplePixelShader(_device.Get(), _context.Get(), psFilepath.c_str()); 

	//load textures
	DirectX::CreateDDSTextureFromFile(_device.Get(), textureFilepath.c_str(), nullptr,  skyboxTexture.GetAddressOf());

	//Setup Rasterizer State
	D3D11_RASTERIZER_DESC rastDesc = {}; 
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_FRONT;
	_device.Get()->CreateRasterizerState(&rastDesc, rasterizerOptions.GetAddressOf());

	//setup depth-stencil state
	D3D11_DEPTH_STENCIL_DESC depthDesc = {}; 
	depthDesc.DepthEnable = true; 
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; 
	_device.Get()->CreateDepthStencilState(&depthDesc, depthBufferOptions.GetAddressOf());
}

Sky::~Sky()
{
	delete pixelShader; 
	delete vertexShader; 
	pixelShader = nullptr; 
	vertexShader = nullptr; 
}

void Sky::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera)
{
	//change render states
	context.Get()->RSSetState(rasterizerOptions.Get()); 
	context.Get()->OMSetDepthStencilState(depthBufferOptions.Get(),0);

	vertexShader->SetShader(); 
	pixelShader->SetShader(); 

	//send data to pixel shader
	pixelShader->SetShaderResourceView("skyboxTexture", skyboxTexture.Get());
	pixelShader->SetSamplerState("samplerOptions", samplerOptions.Get());
	pixelShader->CopyAllBufferData();

	//send data to the vertex shader
	SimpleVertexShader* vsData = vertexShader; 
	vsData->SetMatrix4x4("view", camera->GetViewMatrix());
	vsData->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	vsData->CopyAllBufferData();

	//draw
		// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	//  - for this demo, this step *could* simply be done once during Init(),
	//    but I'm doing it here because it's often done multiple times per frame
	//    in a larger application/game
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, skyGeometry->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(skyGeometry->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(
		skyGeometry->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	//reset render states
	context.Get()->RSSetState(0);
	context.Get()->OMSetDepthStencilState(0, 0);

}
