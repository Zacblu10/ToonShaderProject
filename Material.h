#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl/client.h>
#include "SimpleShader.h"



class Material
{
public:
	Material(DirectX::XMFLOAT4 _colorTint, SimplePixelShader* _pixelShader, SimpleVertexShader* _vertextShader, ID3D11ShaderResourceView* _texture, ID3D11ShaderResourceView* _normalMap, ID3D11ShaderResourceView* _roughMap, ID3D11ShaderResourceView* _metalMap, ID3D11SamplerState* _samplerState);

	//getters 
	DirectX::XMFLOAT4 GetColorTint();
	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();
	ID3D11ShaderResourceView* GetSRV(); 
	ID3D11ShaderResourceView* GetNormalMap();
	ID3D11ShaderResourceView* GetRoughness();
	ID3D11ShaderResourceView* GetMetalness();
	ID3D11SamplerState* GetSamplerState();



	//setters
	void SetColorTint(DirectX::XMFLOAT4 _colorTint);


private:
	DirectX::XMFLOAT4 colorTint;

	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertextShader;

	ID3D11ShaderResourceView* texture;
	ID3D11ShaderResourceView* normalMap;
	ID3D11ShaderResourceView* roughMap; 
	ID3D11ShaderResourceView* metalMap; 
	ID3D11SamplerState* samplerState; 
};

