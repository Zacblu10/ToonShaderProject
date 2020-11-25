#pragma once

#include <DirectXMath.h>

struct Light
{ 
	DirectX::XMFLOAT3 ambientColor; 
	float padding1; 
	DirectX::XMFLOAT3 diffuseColor; 
	float padding2; 
	DirectX::XMFLOAT3 dirOrPos;
	//0 = directional
	//1 = point
	int type;
	float intensity;
};