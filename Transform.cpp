#include "Transform.h"

using namespace DirectX;

Transform::Transform()
{
	position = XMFLOAT3(0, 0, 0); 
	scale = XMFLOAT3(1, 1, 1);
	rotation = XMFLOAT3(0, 0, 0);

	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	
	updateMatrix = false;
}

Transform::Transform(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT3 _rotation, DirectX::XMFLOAT3 _scale)
{
	position = _position;
	scale = _scale;
	rotation = _rotation;

	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());

	updateMatrix = true;
}

#pragma region Setters
void Transform::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z); //set the values
	updateMatrix = true; //we will need to recalculate the world matrix now
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	rotation = XMFLOAT3(pitch, yaw, roll);//set the values
	updateMatrix = true;//we will need to recalculate the world matrix now
}

void Transform::SetScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);//set the values
	updateMatrix = true; //we will need to recalculate the world matrix now
}
#pragma endregion Setters


#pragma region Getters
XMFLOAT3 Transform::GetPosition()
{
	return position;
}

XMFLOAT3 Transform::GetRotation()
{
	return rotation;
}

XMFLOAT3 Transform::GetScale()
{
	return scale;
}

XMFLOAT4X4 Transform::GetWorldMatrix()
{
	if (updateMatrix)//if the matrix has been modified
	{
		//setup matricies
		XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		XMMATRIX  scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

		//Apply Translation, Rotation, the Scale
		XMMATRIX world = scaleMatrix * rotationMatrix * translationMatrix;

		//Store the matrix value as a XMFLOAT4x4
		XMStoreFloat4x4(&worldMatrix, world);
	}
	return worldMatrix;
}
#pragma endregion Getters


void Transform::MoveAbsolute(float x, float y, float z)
{
	updateMatrix = true; //matrix will need to be updated
	position.x += x; 
	position.y += y; 
	position.z += z; 
}

void Transform::MoveRelative(float x, float y, float z)
{
	updateMatrix = true; //matrix will need to be updated
	//Get the vector to move on 
	XMVECTOR direction = XMVectorSet(x, y, z, 0); 
	//Get quaternion rotation
	XMVECTOR qRotation = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	//rotate the direction vector
	direction = XMVector3Rotate(direction, qRotation);

	//do the math
	XMVECTOR currentPos = XMLoadFloat3(&position);
	currentPos += direction; 
	XMStoreFloat3(&position, currentPos);
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	updateMatrix = true; //matrix will need to be updated
	rotation.x += pitch; 
	rotation.y += yaw; 
	rotation.z += roll; 
}

void Transform::Scale(float x, float y, float z)
{
	updateMatrix = true; //matrix will need to be updated
	scale.x += x; 
	scale.y += y; 
	scale.z += z; 
}
