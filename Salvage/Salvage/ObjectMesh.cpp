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
