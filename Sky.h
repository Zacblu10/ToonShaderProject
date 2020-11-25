#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <DDSTextureLoader.h>


#include "Mesh.h"
#include "SimpleShader.h"
#include "Camera.h"

class Sky
{
public:
	// constructor
	Sky(Mesh * skyMesh, Microsoft::WRL::ComPtr<ID3D11SamplerState> _samplerOptions, Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context, std::wstring textureFilepath, std::wstring vsFilepath, std::wstring psFilepath);
	//destructor
	~Sky();	

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context, Camera* camera); 

private: 
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions; 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyboxTexture; 
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthBufferOptions; 
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerOptions; 

	Mesh* skyGeometry; 
	SimplePixelShader* pixelShader; 
	SimpleVertexShader* vertexShader; 
};

