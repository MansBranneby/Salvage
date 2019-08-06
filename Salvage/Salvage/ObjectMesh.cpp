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

void ObjectMesh::draw(GraphicResources* graphicResources)
{
	UINT offset = 0;
	UINT vertexSize = sizeof(Vertex);

	graphicResources->getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
	graphicResources->getDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < _textures.size(); i++)
		graphicResources->getDeviceContext()->PSSetShaderResources(i, 1, &_textures[i]._texture);


	graphicResources->getDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
}
