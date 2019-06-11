#pragma once

#include <vector>
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model
{
private:
	std::vector<Mesh> _meshes;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
public:
	Model();
	~Model();
};