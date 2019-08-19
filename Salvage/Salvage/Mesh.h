#pragma once
#define NOMINMAX

#include "GraphicResources.h"
#include "Texture.h"
#include "Vertex.h"
#include "TerrainVertex.h"
#include <vector>

class Mesh
{
private:
protected:
	std::vector<int> _indices;
	std::vector<Texture> _textures;

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;

	void createBuffers(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<int> indices);
	void createBuffers(ID3D11Device* device, std::vector<TerrainVertex> vertices, std::vector<int> indices);

public:
	Mesh();
	Mesh(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<int> indices, std::vector<Texture> textures);
	Mesh(ID3D11Device* device, std::vector<TerrainVertex> vertices, std::vector<int> indices, std::vector<Texture> textures);
	virtual ~Mesh() = 0;

	virtual void draw(GraphicResources* graphicResources) = 0;
};