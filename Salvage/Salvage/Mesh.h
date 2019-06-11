#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <vector>

class Mesh
{
private:
	std::vector<Vertex> _vertices;

public:
	Mesh();
	Mesh(ID3D11Device* device);
	~Mesh();
};