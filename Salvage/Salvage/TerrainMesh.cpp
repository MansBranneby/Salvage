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
