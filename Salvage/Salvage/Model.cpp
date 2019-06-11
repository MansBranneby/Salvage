#include "Model.h"

void Model::processNode(ID3D11Device* device, aiNode * node, const aiScene * scene)
{
	//Cycle all meshes for current node
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(this->processMesh(device, mesh, scene));
	}

	//Go down tree and process all nodes
	for (int i = 0; i < node->mNumChildren; i++)
		this->processNode(device, node->mChildren[i], scene);
}

Mesh Model::processMesh(ID3D11Device* device, aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::vector<Texture> textures;

	//Loop vertices
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;
		
		//Texturecoord here

		//Save vertex
		vertices.push_back(vertex);
	}

	//Loop faces
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//Textures
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> texture = loadTextures(material, aiTextureType_DIFFUSE, scene);
		textures.insert(textures.end(), texture.begin(), texture.end());
	}

	return Mesh(device, vertices, indices);
}

std::vector<Texture> Model::loadTextures(aiMaterial* material, aiTextureType textureType, const aiScene * scene)
{
	//Load texture
	return std::vector<Texture>();
}

Model::Model()
{
}

Model::Model(ID3D11Device* device)
{
	//Load model from file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(".\\Resources\\Models\\moremodels.dae", aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	
	processNode(device, scene->mRootNode, scene);
}

Model::~Model()
{
}

void Model::draw(ID3D11DeviceContext* deviceContext)
{
	for (int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i].draw(deviceContext);
	}
}
