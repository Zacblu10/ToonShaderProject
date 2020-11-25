#include "Game.h"
#include "Vertex.h"

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	pixelShader = 0;
	vertexShader = 0;
	psNormalMap = 0;
	vsNormalMap = 0;
	camera = 0;
	dirLight1 = {};
	dirLight2 = {};
	dirLight3 = {};
	pointLight1 = {};
	skybox = nullptr; 
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object created in Game
	
	//delete shaders
	delete vertexShader; 
	delete pixelShader;
	vertexShader = nullptr; 
	pixelShader = nullptr; 
	delete vsNormalMap; 
	delete psNormalMap; 
	psNormalMap = nullptr; 
	vsNormalMap = nullptr;

	//delete meshes
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
	//delete entites
	for (int i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	//delete materials
	for (int i = 0; i < materials.size(); i++) {
		delete materials[i];
	}

	delete camera;
	delete skybox;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateBasicGeometry();
	
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//create camera 
	camera = new Camera(
		XMFLOAT3(0, 0, -10), //position
		XMFLOAT3(0, 0, 0), //rotation
		(float)this->width / this->height,//aspect ration
		XM_PI * 0.25f, //fov (not currently used)
		.1f, //near clipping plane
		100, //far clipping plane
 		5, //movement speed
		5); //rotation speed

	////create lights
	//lights.push_back({}); 
	//lights[0].ambientColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	//lights[0].diffuseColor = XMFLOAT3(0.0f, 0.0f, 0.8f);
	//lights[0].dirOrPos = XMFLOAT3(0, -1, 0);
	//lights[0].type = 0;
	//lights[0].intensity = 5;

	//lights.push_back({});
	//lights[1].ambientColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	//lights[1].diffuseColor = XMFLOAT3(0.0f, 0.0f, 0.8f);
	//lights[1].dirOrPos = XMFLOAT3(0, 1, 0);
	//lights[1].type = 0;
	//lights[1].intensity = 5;

	//lights.push_back({});
	//lights[2].ambientColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	//lights[2].diffuseColor = XMFLOAT3(0.0f, 0.0f, 0.8f);
	//lights[2].dirOrPos =  XMFLOAT3(1, 0, 1);
	//lights[2].type = 0;
	//lights[2].intensity = 5;

	lights.push_back({});
	lights[0].ambientColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	lights[0].diffuseColor = XMFLOAT3(0.0f, 0.0f, 0.8f);
	lights[0].dirOrPos = XMFLOAT3(0, 0, -3);
	lights[0].type = 1;
	lights[0].intensity = 10;

	lights.push_back({});
	lights[1].ambientColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	lights[1].diffuseColor = XMFLOAT3(0.0f, 0.0f, 0.8f);
	lights[1].dirOrPos = XMFLOAT3(-5, 0, 0);
	lights[1].type = 0;
	lights[1].intensity = 10;



}


// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{	
	//vertexShader = new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"VertexShader.cso").c_str());
	//pixelShader = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShader.cso").c_str());
	vsNormalMap = new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"NormalMapVS.cso").c_str());
	psNormalMap = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"NormalMapPS.cso").c_str());
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 yellow = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);



	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself

	Vertex vertices[] =//shape 2: pentagon
	{
		{ XMFLOAT3(-0.2f, +0.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0)},
		{ XMFLOAT3(-0.0f, +0.2f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.0f, +0.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.2f, +0.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.1f, -0.3f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.1f, -0.3f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) }
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int indices[] = { 0, 1, 2,
								2, 1, 3,
								3, 4, 2,
								2, 4, 5,
								5, 0, 2
								};
	
	//add the mesh to the mesh list
	//meshes.push_back(new Mesh(vertices, 6, indices, 15, device));
	
	//load OBJs 
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/cone.obj").c_str(), device));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/cylinder.obj").c_str(), device));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/helix.obj").c_str(), device));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/torus.obj").c_str(), device));

	//load textures
	//CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/jeans.jpg").c_str(), nullptr, jean.GetAddressOf());
	//CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/marble.jpg").c_str(), nullptr, marble.GetAddressOf());
	//CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/rock.png").c_str(), nullptr, rock.GetAddressOf());
	//CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/rock_normals.png").c_str(), nullptr, rockNormal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/floor_albedo.png").c_str(), nullptr, floor_albedo.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/floor_normals.png").c_str(), nullptr, floor_normal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/floor_roughness.png").c_str(), nullptr, floor_rough.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/floor_metal.png").c_str(), nullptr, floor_metal.GetAddressOf());

	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/bronze_albedo.png").c_str(), nullptr, bronze_albedo.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/bronze_normals.png").c_str(), nullptr, bronze_normal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/bronze_roughness.png").c_str(), nullptr, bronze_rough.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/bronze_metal.png").c_str(), nullptr, bronze_metal.GetAddressOf());

	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/paint_albedo.png").c_str(), nullptr, paint_albedo.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/paint_normals.png").c_str(), nullptr, paint_normal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/paint_roughness.png").c_str(), nullptr, paint_rough.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/paint_metal.png").c_str(), nullptr, paint_metal.GetAddressOf());

	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/cobblestone_albedo.png").c_str(), nullptr, cobblestone_albedo.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/cobblestone_normals.png").c_str(), nullptr, cobblestone_normal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/cobblestone_roughness.png").c_str(), nullptr, cobble_rough.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/PBR/cobblestone_metal.png").c_str(), nullptr, cobblestone_metal.GetAddressOf());

	//set up sampler state 
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_ANISOTROPIC; 
	desc.MaxAnisotropy = 8; 
	desc.MaxLOD = D3D11_FLOAT32_MAX; 

	device->CreateSamplerState(&desc, &samplerState);

	//create the sky
	skybox = new Sky(meshes[1], samplerState, device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/SpaceCubeMap.dds"), GetFullPathTo_Wide(L"SkyboxVS.cso"), GetFullPathTo_Wide(L"SkyboxPS.cso"));

	//PBR Materials
	materials.push_back(new Material(white, psNormalMap, vsNormalMap, floor_albedo.Get(), floor_normal.Get(), floor_rough.Get(), floor_metal.Get(), samplerState.Get())); 
	materials.push_back(new Material(white, psNormalMap, vsNormalMap, bronze_albedo.Get(), bronze_normal.Get(), bronze_rough.Get(), bronze_metal.Get(), samplerState.Get()));
	materials.push_back(new Material(white, psNormalMap, vsNormalMap, paint_albedo.Get(), paint_normal.Get(), paint_rough.Get(), paint_metal.Get(), samplerState.Get()));
	materials.push_back(new Material(white, psNormalMap, vsNormalMap, cobblestone_albedo.Get(), cobblestone_normal.Get(), cobble_rough.Get(), cobblestone_metal.Get(), samplerState.Get()));


	//create materials
	//materials.push_back(new Material(white, pixelShader, vertexShader,1, marble.Get(), 0, samplerState.Get()));
	//materials.push_back(new Material(white, pixelShader, vertexShader,100, marble.Get(),0, samplerState.Get()));
	//materials.push_back(new Material(white, pixelShader, vertexShader,100, jean.Get(),0, samplerState.Get()));
	//materials.push_back(new Material(white, pixelShader, vertexShader,1000, jean.Get(),0, samplerState.Get()));

	//create normal map materials
	//materials.push_back(new Material(white, psNormalMap, vsNormalMap, 1000, rock.Get(), rockNormal.Get() , samplerState.Get()));

	//create entities
	entities.push_back(new Entity(meshes[4], materials[0]));
	entities.push_back(new Entity(meshes[4], materials[1]));
	entities.push_back(new Entity(meshes[4], materials[2]));
	entities.push_back(new Entity(meshes[4], materials[3]));

	//move entities
	entities[0]->GetTransform()->SetPosition(-1, 0, 0);
	entities[1]->GetTransform()->SetPosition(1, 0, 0);
	entities[2]->GetTransform()->SetPosition(-3, 0, 0);
	entities[3]->GetTransform()->SetPosition(3, 0, 0);

	//scale the entities 
	entities[0]->GetTransform()->SetScale(2, 2, 2);
	entities[1]->GetTransform()->SetScale(2, 2, 2);
	entities[2]->GetTransform()->SetScale(2, 2, 2);
	entities[3]->GetTransform()->SetScale(2, 2, 2);



	//create entities
	//entities.push_back(new Entity(meshes[0], materials[3]));
	//entities.push_back(new Entity(meshes[1], materials[1]));
	//entities.push_back(new Entity(meshes[2], materials[4]));
	//entities.push_back(new Entity(meshes[3], materials[3]));
	//entities.push_back(new Entity(meshes[4], materials[4]));
	//entities.push_back(new Entity(meshes[5], materials[1]));

	//scale the helix
	//entities[3]->GetTransform()->SetScale(.5f, .5f, .5f);

	//move the entities
	//entities[0]->GetTransform()->SetPosition(-5, 0, 0);
	//entities[1]->GetTransform()->SetPosition(-3, 0, 0);
	//entities[2]->GetTransform()->SetPosition(-1, 0, 0);
	//entities[3]->GetTransform()->SetPosition(1, 0, 0);
	//entities[4]->GetTransform()->SetPosition(3, 0, 0);
	//entities[5]->GetTransform()->SetPosition(5, 0, 0);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
	if (camera != 0)
	{
		camera->UpdateProjectionMatrix((float)this->width / this->height);
	}
	
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	
	//update camera
	if (camera != 0)
	{
		camera->Update(deltaTime, this->hWnd);
	}
	

	//move entities
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->GetTransform()->Rotate(0, .2f * deltaTime, 0);
	}
	

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//Draw stuff here-------------------------------------
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->Draw(context, camera, lights);
	}
	//draw the sky 
	skybox->Draw(context.Get(), camera);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}