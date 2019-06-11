#pragma once
#define NOMINMAX
#include <vector>
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model: public Drawable
{
private:
	std::vector<Mesh> _meshes;

	void processNode(ID3D11Device* device, aiNode* node, const aiScene* scene);
	Mesh processMesh(ID3D11Device* device, aiMesh* mesh, const aiScene* scene);
public:
	Model();
	Model(ID3D11Device* device);
	~Model();

	void draw(ID3D11DeviceContext* deviceContext);
};