#pragma once

#include "Vertex.h"
#include <vector>

class Mesh
{
private:
	std::vector<Vertex> _vertices;

public:
	Mesh();
	~Mesh();
};