#pragma once
#include <Windows.h>

#include "Transform.h"

class Camera
{
public:
	Camera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 _rotation, float _aspectRatio, float _fov, float _near, float _far, float _moveSpeed, float _lookSpeed);

	//getters 
	DirectX::XMFLOAT4X4 GetViewMatrix(); 
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	DirectX::XMFLOAT3 GetPosition();

	//methods
	void UpdateProjectionMatrix(float aspectRatio);
	void UpdateViewMatrix();
	void Update(float dt, HWND windowHandle); 

private: 
	Transform transform; 
	DirectX::XMFLOAT4X4 view; 
	DirectX::XMFLOAT4X4 projection;

	POINT lastMousePos;
	float fov;
	float clipNear; 
	float clipFar; 
	float moveSpeed; 
	float lookSpeed; 
};

