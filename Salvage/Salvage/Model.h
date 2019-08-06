#pragma once
#define NOMINMAX
#include <vector>
#include <string>
#include <map>

#include <DirectXMath.h>
#include <d3d11.h>
#include "TextureLoader.h" //Används inte i nuvarande version
#include "WICTextureLoader.h" //Denna används

#include "GraphicResources.h"
#include "Mesh.h"
#include "TerrainMesh.h"
#include "ObjectMesh.h"
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "BoundingVolume.h"
#include "OBB.h"

struct BoneInfo
{
	aiMatrix4x4 boneOffset;
	aiMatrix4x4 finalTransformation;
};

class Model
{
private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	std::string _directory;
	std::vector<Mesh*> _meshes;
	std::vector<Texture> _loadedTextures;
	Assimp::Importer _importer;
	const aiScene* _scene;

	//Animation
	DirectX::XMMATRIX _inverseTransform;
	std::map<std::string, int> _boneMapping;
	std::vector<BoneInfo> _boneInfo; //Tror att denna ska vara i Model och inte i Mesh
	//aiNode* findNodeRecursivelyByName(const aiNode* node, aiString channelName);

	// Moving
	aiMatrix4x4 _world;

	//Loading models
	void processNode(ID3D11Device* device, aiNode* node);
	Mesh* processMesh(ID3D11Device* device, aiMesh* mesh);

	//Process textures
	std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType textureType, std::string typeName);
	std::string determineTextureType(aiMaterial* material);
	int getTextureIndex(aiString* str);
	ID3D11ShaderResourceView* getTextureFromModel(int textureIndex);

	//Bounding volume
	BoundingVolume* _boundingVolume = nullptr;

public:
	Model();
	~Model();
	
	std::vector<Mesh*>* getMeshes();
	std::vector<Texture>* getLoadedTextures();
	BoundingVolume* getBoundingVolume();

	void updateTransformation(DirectX::XMFLOAT3 position);
	bool loadModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string filename);
	//void animate(float timeInSec);
	void draw(GraphicResources* graphicResources);
	void drawBoundingVolume(GraphicResources* graphicResources);
};