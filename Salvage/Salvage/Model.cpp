#include "Model.h"

void Model::processNode(aiNode * node, const aiScene * scene)
{
	//Cycle all meshes for current node
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(this->processMesh(mesh, scene));
	}

	//Go down tree and process all nodes
	for (int i = 0; i < node->mNumChildren; i++)
		this->processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	
}

Model::Model()
{
	//Load model from file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(".\\Resources\\Models\\monkey.dae", aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	
	processNode(scene->mRootNode, scene);
}

Model::~Model()
{
}
