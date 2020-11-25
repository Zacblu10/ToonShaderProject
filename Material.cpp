#include "Material.h"

Material::Material(DirectX::XMFLOAT4 _colorTint, SimplePixelShader* _pixelShader, SimpleVertexShader* _vertextShader, ID3D11ShaderResourceView* _texture, ID3D11ShaderResourceView* _normalMap, ID3D11ShaderResourceView* _roughMap, ID3D11ShaderResourceView* _metallMap, ID3D11SamplerState* _samplerState)
{
	colorTint = _colorTint; 
	pixelShader = _pixelShader; 
	vertextShader = _vertextShader;
	texture = _texture; 
	normalMap = _normalMap;
	roughMap = _roughMap; 
	metalMap = _metallMap; 
	samplerState = _samplerState;
}

DirectX::XMFLOAT4 Material::GetColorTint()
{
	return colorTint;
}

SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertextShader; 
}

ID3D11ShaderResourceView* Material::GetSRV()
{
	return texture;
}

ID3D11ShaderResourceView* Material::GetNormalMap()
{
	return normalMap;
}

ID3D11ShaderResourceView* Material::GetRoughness()
{
	return roughMap;
}

ID3D11ShaderResourceView* Material::GetMetalness()
{
	return metalMap;
}

ID3D11SamplerState* Material::GetSamplerState()
{
	return samplerState;
}

void Material::SetColorTint(DirectX::XMFLOAT4 _colorTint)
{
	colorTint = _colorTint;
}
