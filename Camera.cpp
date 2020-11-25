#include "Camera.h"
#include <stdio.h>

using namespace DirectX;

Camera::Camera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, float _aspectRatio, float _fov, float _near, float _far, float _moveSpeed, float _lookSpeed)
{
	//create the transform
	transform = Transform(position, rotation, XMFLOAT3(0, 0, 0));
	//set initial values
	
	fov = _fov;
	clipNear = _near;
	clipFar = _far;;
	moveSpeed = _moveSpeed;
	lookSpeed = _lookSpeed;

	UpdateViewMatrix(); 
	UpdateProjectionMatrix(_aspectRatio);

}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return view;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projection;
}

DirectX::XMFLOAT3 Camera::GetPosition()
{
	return transform.GetPosition();
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	XMStoreFloat4x4(&projection, XMMatrixPerspectiveFovLH(fov, aspectRatio, clipNear, clipFar));
}

void Camera::UpdateViewMatrix()
{
	XMVECTOR position = XMVectorSet(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z,0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);

	XMVECTOR forward = XMVectorSet(0, 0, 1, 0);
	XMVECTOR qRotation = XMQuaternionRotationRollPitchYaw(transform.GetRotation().x, transform.GetRotation().y, transform.GetRotation().z);
	forward = XMVector3Rotate(forward, qRotation);
	

	XMMATRIX newView = XMMatrixLookToLH(position, forward, up);
	XMStoreFloat4x4(&view, newView);
}

void Camera::Update(float dt, HWND windowHandle)
{
	//keyboard input
	if (GetAsyncKeyState('W') & 0x8000) { transform.MoveRelative(0, 0, moveSpeed * dt);  } //Forward
	if (GetAsyncKeyState('S') & 0x8000) { transform.MoveRelative(0, 0, -moveSpeed * dt); } //Backwards
	if (GetAsyncKeyState('A') & 0x8000) { transform.MoveRelative(-moveSpeed * dt, 0, 0); }//Left
	if (GetAsyncKeyState('D') & 0x8000) { transform.MoveRelative(moveSpeed * dt, 0, 0); }//Right
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) { transform.MoveAbsolute(0, moveSpeed * dt, 0); }//Up
	if (GetAsyncKeyState('X') & 0x8000) { transform.MoveAbsolute(0, -moveSpeed * dt, 0); }//Down

	//mouse input
	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(windowHandle, &mousePos);

	//rotate the camera
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		float x = (mousePos.x - lastMousePos.x) * lookSpeed * dt;
		float y = (mousePos.y - lastMousePos.y)* lookSpeed * dt;

		transform.Rotate(y, x, 0);
	}
	
	//store the mousePos this frame
	lastMousePos = mousePos;
	
	//update the view matrix
	UpdateViewMatrix();
}
