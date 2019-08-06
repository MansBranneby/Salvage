#include "TerrainMesh.h"

TerrainMesh::TerrainMesh()
{
}

TerrainMesh::TerrainMesh(ID3D11Device * device, std::vector<TerrainVertex> vertices, std::vector<int> indices, std::vector<Texture> textures)
	:Mesh(device, vertices, indices, textures)
{
	_vertices = vertices;
}

TerrainMesh::~TerrainMesh()
{
}

void TerrainMesh::draw(GraphicResources* graphicResources)
{
	UINT offset = 0;
	UINT vertexSize = sizeof(TerrainVertex);
	

	graphicResources->getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
	graphicResources->getDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	graphicResources->getDeviceContext()->VSSetShaderResources(5, 1, &_textures[5]._texture);
	graphicResources->getDeviceContext()->DSSetShaderResources(5, 1, &_textures[5]._texture);

	for (int i = 0; i < _textures.size(); i++)
		graphicResources->getDeviceContext()->PSSetShaderResources(i, 1, &_textures[i]._texture);

	graphicResources->getDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
}
