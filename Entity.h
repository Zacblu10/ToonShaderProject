#pragma once
#include <memory>

#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "Lights.h"

class Entity
{
public: 
	Entity(Mesh* _mesh, Material* _material);

	//getters
	Mesh* GetMesh(); 
	Transform* GetTransform(); 

	//methods
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera, std::vector<Light> lights);
private: 
	Transform transform; 
	Mesh* mesh; 
	Material* material;
};

