#include "Entity.h"

Entity::Entity(Mesh*_mesh, Material* _material)
{
	mesh = _mesh;
	material = _material;
}

Mesh* Entity::GetMesh()
{
	return mesh;
}

Transform* Entity::GetTransform()
{
	return &transform;
}

void Entity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera, std::vector<Light> lights)
{
	//activate shaders
	//context->VSSetShader(material->GetVertexShader().Get(), 0, 0);
	//context->PSSetShader(material->GetPixelShader().Get(), 0, 0);
	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();

	//set external data
	SimpleVertexShader* vsData = material->GetVertexShader();
	vsData->SetFloat4("colorTint", material->GetColorTint());
	vsData->SetMatrix4x4("world", transform.GetWorldMatrix());
	vsData->SetMatrix4x4("view", camera->GetViewMatrix());
	vsData->SetMatrix4x4("projection",camera->GetProjectionMatrix());

	vsData->CopyAllBufferData();

	//send material info
	material->GetPixelShader()->SetSamplerState("SamplerOptions",material->GetSamplerState());
	material->GetPixelShader()->SetShaderResourceView("Albedo", material->GetSRV());
	//if the texture has a normal map, send the texture
	if (material->GetNormalMap() != 0)
	{
		material->GetPixelShader()->SetShaderResourceView("NormalMap", material->GetNormalMap());
	}
	material->GetPixelShader()->SetShaderResourceView("RoughnessMap", material->GetRoughness());
	material->GetPixelShader()->SetShaderResourceView("MetalnessMap", material->GetMetalness());
	//lighting 
	material->GetPixelShader()->SetData("light1", &lights[0], sizeof(Light));
	material->GetPixelShader()->SetData("light2", &lights[1], sizeof(Light));
	material->GetPixelShader()->SetFloat3("cameraPosition", camera->GetPosition());

	material->GetPixelShader()->CopyAllBufferData();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	//  - for this demo, this step *could* simply be done once during Init(),
	//    but I'm doing it here because it's often done multiple times per frame
	//    in a larger application/game
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	

}
