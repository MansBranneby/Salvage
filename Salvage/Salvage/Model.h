#pragma once

#include <vector>
#include "Mesh.h"

class Model
{
private:
	std::vector<Mesh> _meshes;
public:
	Model();
	~Model();
};