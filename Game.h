#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <memory> //unique_ptr
#include <vector>
#include <WICTextureLoader.h>

#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Material.h"
#include "Lights.h"
#include "Sky.h"


class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();

	
	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	
	// Shaders and shader-related constructs
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* psNormalMap;
	SimpleVertexShader* vsNormalMap;
	
	//lights
	std::vector<Light> lights; 
	Light dirLight1; 
	Light dirLight2;
	Light dirLight3;
	Light pointLight1;

	//Mesh Objects
	std::vector<Mesh*> meshes; 

	//Entities
	std::vector<Entity*> entities;

	//Materials
	std::vector<Material*> materials;

	//camera
	Camera* camera; 

	//texture stuff 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> floor_albedo;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> floor_normal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> floor_rough;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> floor_metal;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bronze_albedo;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bronze_normal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bronze_rough;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bronze_metal;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> paint_albedo;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> paint_normal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> paint_rough;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> paint_metal;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cobblestone_albedo;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cobblestone_normal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cobble_rough;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cobblestone_metal;
//	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> marble;
//	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rock;
//	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rockNormal;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	//skybox stuff 
	Sky* skybox; 

};

