#pragma once
#include "Mesh.h"

class TerrainMesh : public Mesh
{
private:
	std::vector<TerrainVertex>_vertices;

public:
	TerrainMesh();
	TerrainMesh(ID3D11Device* device, std::vector<TerrainVertex> vertices, std::vector<int> indices, std::vector<Texture> textures);
	~TerrainMesh();

	void draw(GraphicResources* graphicResources);
};