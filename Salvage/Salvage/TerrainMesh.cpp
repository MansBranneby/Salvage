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

void TerrainMesh::draw(ID3D11DeviceContext * deviceContext, ID3D11Buffer * transformationBuffer)
{
	UINT offset = 0;
	UINT vertexSize = sizeof(TerrainVertex);

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &transformationBuffer);

	deviceContext->VSSetShaderResources(5, 1, &_textures[5]._texture);
	deviceContext->DSSetShaderResources(5, 1, &_textures[5]._texture);

	for (int i = 0; i < _textures.size(); i++)
		deviceContext->PSSetShaderResources(i, 1, &_textures[i]._texture);

	deviceContext->DrawIndexed((UINT)_indices.size(), 0, 0);
}
