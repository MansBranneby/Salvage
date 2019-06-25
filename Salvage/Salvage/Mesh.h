#pragma once
#define NOMINMAX

#include "Drawable.h"
#include "Texture.h"
#include "Vertex.h"
#include <vector>

struct VertexBoneData
{
	int* ids; // NUM_BONES_PER_VERTEX?
	float weights[4]; // NUM_BONES_PER_VERTEX?
};

class Mesh: public Drawable
{
private:
	std::vector<Vertex> _vertices;
	std::vector<int> _indices;
	std::vector<Texture> _textures;

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;

	void createBuffers(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<int> indices);

public:
	Mesh();
	Mesh(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<int> indices, std::vector<Texture> textures);
	~Mesh();

	void draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* transformationBuffer);
};