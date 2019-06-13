#pragma once
#define NOMINMAX
#include <vector>
#include <string>
#include <map>

#include <DirectXMath.h>
#include <d3d11.h>
#include "TextureLoader.h" //Används inte i nuvarande version
#include "WICTextureLoader.h" //Denna används

#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

struct BoneInfo
{
	aiMatrix4x4 boneOffset;
	aiMatrix4x4 finalTransformation;
};

class Model: public Drawable
{
private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	std::string _directory;
	std::vector<Mesh> _meshes;
	std::vector<Texture> _loadedTextures;

	//Animation
	DirectX::XMMATRIX _inverseTransform;
	std::map<std::string, int> _boneMapping;
	std::vector<BoneInfo> _boneInfo; //Tror att denna ska vara i Model och inte i Mesh

	//Loading models
	void processNode(ID3D11Device* device, aiNode* node, const aiScene* scene);
	Mesh processMesh(ID3D11Device* device, aiMesh* mesh, const aiScene* scene);

	//Process textures
	std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType textureType, std::string typeName, const aiScene* scene);
	std::string determineTextureType(const aiScene* scene, aiMaterial* material);
	int getTextureIndex(aiString* str);
	ID3D11ShaderResourceView* getTextureFromModel(const aiScene* scene, int textureIndex);

public:
	Model();
	~Model();

	bool loadModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filename);
	void draw(ID3D11DeviceContext* deviceContext);
};