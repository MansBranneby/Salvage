#pragma once
#include "Mesh.h"

class ObjectMesh : public Mesh
{
private:
	std::vector<Vertex> _vertices;

public:
	ObjectMesh();
	ObjectMesh(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<int> indices, std::vector<Texture> textures);
	~ObjectMesh();

	void draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* transformationBuffer);
};