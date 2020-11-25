#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include "Vertex.h"
#include <fstream>
#include <vector>

class Mesh
{
public:
	//Constructors
	Mesh(Vertex* _vertices, int _numVertices, unsigned int _indices[], int _numIndices, Microsoft::WRL::ComPtr<ID3D11Device> _device); 
	Mesh(const char* fileName, Microsoft::WRL::ComPtr<ID3D11Device> _device);
	//Destructor
	~Mesh(); 
	//Methods
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer(); 
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount(); 

private:

	//methods
	void CreateBuffers(Vertex* _vertices, int _numVertices, unsigned int _indices[], int _numIndices, Microsoft::WRL::ComPtr<ID3D11Device> _device);
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	//fields
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int indexCount; 
};

