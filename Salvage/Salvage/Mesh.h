#pragma once
#define NOMINMAX
#include <d3d11.h>
#include "Drawable.h"
#include "Vertex.h"
#include <vector>

class Mesh: public Drawable
{
private:
	std::vector<Vertex> _vertices;
	std::vector<int> _indices;

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;

	void createBuffers(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<int> indices);

public:
	Mesh();
	Mesh(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<int> indices);
	~Mesh();

	void draw(ID3D11DeviceContext* deviceContext);
};