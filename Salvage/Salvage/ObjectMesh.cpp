#include "ObjectMesh.h"

ObjectMesh::ObjectMesh()
{
}

ObjectMesh::ObjectMesh(ID3D11Device * device, std::vector<Vertex> vertices, std::vector<int> indices, std::vector<Texture> textures)
	:Mesh(device, vertices, indices, textures)
{
	_vertices = vertices;
}

ObjectMesh::~ObjectMesh()
{
}

void ObjectMesh::draw(ID3D11DeviceContext * deviceContext, ID3D11Buffer * transformationBuffer)
{
	UINT offset = 0;
	UINT vertexSize = sizeof(Vertex);

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &transformationBuffer);

	for (int i = 0; i < _textures.size(); i++)
		deviceContext->PSSetShaderResources(i, 1, &_textures[i]._texture);


	deviceContext->DrawIndexed((UINT)_indices.size(), 0, 0);
}
