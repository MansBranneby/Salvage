#include "Model.h"

//aiNode * Model::findNodeRecursivelyByName(const aiNode* node, aiString channelName)
//{
//	aiNode thisNode = *node;
//	aiNode* returnNode;
//	//Check name for current node
//	if (node->mName == channelName)
//		returnNode = &thisNode;
//	else
//		returnNode = NULL;
//	//Go down tree and process all nodes
//	if (node->mNumChildren && returnNode == NULL)
//	{
//		for (size_t i = 0; i < node->mNumChildren && returnNode == NULL; i++)
//		{
//			returnNode = findNodeRecursivelyByName(node->mChildren[i], channelName);
//
//		}
//	}
//	if (node->mName == channelName)
//	{
//
//	}
//	return returnNode;
//
//	//aiNode* returnNode;
//	////Check name for current node
//	//if (node->mName == channelName)
//	//{
//	//	returnNode = node;
//	//	return node;
//	//}
//	//else
//	//	returnNode = NULL;
//	////Go down tree and process all nodes
//	//if (node->mNumChildren && returnNode == NULL)
//	//{
//	//	for (size_t i = 0; i < node->mNumChildren && returnNode == NULL; i++)
//	//	{
//	//		return findNodeRecursivelyByName(node->mChildren[i], channelName);
//
//	//	}
//	//}
//	//if (node->mName == channelName)
//	//{
//
//	//}
//	//return returnNode;
//}

void Model::processNode(ID3D11Device* device, aiNode * node)
{
	//Cycle all meshes for current node
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = _scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(this->processMesh(device, mesh));
	}

	//Go down tree and process all nodes
	for (size_t i = 0; i < node->mNumChildren; i++)
		this->processNode(device, node->mChildren[i]);
}

std::string texType;

Mesh* Model::processMesh(ID3D11Device* device, aiMesh * mesh)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::vector<Texture> textures;

	//std::vector<VertexBoneData> bones;
	//bones.resize(mesh->mNumVertices); // Kanske r�tt? Hoppas det �r numVer f�r mesh och inte f�r scene...
	//int numBones = 0; //Kom ih�g att skicka denna till Mesh konstruktorn

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = _scene->mMaterials[mesh->mMaterialIndex];
		if (texType.empty())
			texType = determineTextureType(material);
	}

	// Bounding volume
	DirectX::XMFLOAT3 maxCoordinates = { 0, 0, 0 };
	DirectX::XMFLOAT3 minCoordinates = { 0, 0, 0 };
	// Loop vertices
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// Vertex pos
		vertex._position.x = mesh->mVertices[i].x;
		vertex._position.y = mesh->mVertices[i].y;
		vertex._position.z = mesh->mVertices[i].z;
		
		// Vertex normals
		vertex._normal.x = mesh->mNormals[i].x;
		vertex._normal.y = mesh->mNormals[i].y;
		vertex._normal.z = mesh->mNormals[i].z;
		
		// Vertex texture coordinates
		if (mesh->mTextureCoords[0])
		{
			vertex._textureCoords.x = (float)mesh->mTextureCoords[0][i].x; //Jag antar att diffuse texture ligger p� index 0.
			vertex._textureCoords.x = (float)mesh->mTextureCoords[0][i].y; //S� n�r vi vill ha normal map ligger den troligen p� ett annat index.
		}

		//Save vertex
		vertices.push_back(vertex);

		// BOUNDING VOLUME //
		// Look for maximum XYZ coordinates for OBB
		maxCoordinates.x = std::max(maxCoordinates.x, vertex._position.x);
		maxCoordinates.y = std::max(maxCoordinates.y, vertex._position.y);
		maxCoordinates.z = std::max(maxCoordinates.z, vertex._position.z);
		// Look for minimum XYZ coordinates for OBB
		minCoordinates.x = std::min(minCoordinates.x, vertex._position.x);
		minCoordinates.y = std::min(minCoordinates.y, vertex._position.y);
		minCoordinates.z = std::min(minCoordinates.z, vertex._position.z);
	}
	// Create bounding volume
	_boundingVolume = new OBB(device, minCoordinates, maxCoordinates);

	//Loop faces
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (size_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//Textures
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = _scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> texture = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), texture.begin(), texture.end());
	}

	//Load bonesinfo into vertices
	std::vector<int> index;
	std::vector<float> weights;

	//Loop through each vertex
	if (mesh->HasBones())
	{
		for (size_t i = 0; i < vertices.size(); i++)
		{
			index.clear();
			weights.clear();
			//Loop through each bone
			for (size_t j = 0; j < mesh->mNumBones; j++)
			{
				for (size_t k = 0; k < mesh->mBones[j]->mNumWeights; k++)
				{
					//Check if vertex is affected by bone
					if (mesh->mBones[j]->mWeights[k].mVertexId == i)
					{
						index.push_back((int)j);
						weights.push_back(mesh->mBones[j]->mWeights[k].mWeight);
					}
				}
			}
			//Pick top 4 weights for shader
			int nrOfweights = 0; //not always four
			for (size_t j = 0; j < 4 && weights.size() != 0; j++) //H�r kan det nog krascha om en vertex har f�rre �n 4 weights
			{
				int maxIndex = -1;
				float maxValue = -1;
				int eraseIndex = -1;
				for (size_t k = 0; k < weights.size(); k++)
				{
					if (weights[k] > maxValue)
					{
						maxValue = weights[k];
						maxIndex = index[k];
						eraseIndex = (int)k;
					}
				}
				//Insert vertex boneinfo
				vertices[i].boneIndices[j] = (float)maxIndex;
				vertices[i].weights[j] = maxValue;
				nrOfweights++;
				weights.erase(weights.begin() + eraseIndex);
				index.erase(index.begin() + eraseIndex);
			}
			//Calculate weights for top 4. (must add up to 1)
			float prevPer = 0;
			for (size_t j = 0; j < nrOfweights; j++) //Because nrOfWeights not always 4.
				prevPer += vertices[i].weights[j];

			float missPer = 1 - prevPer;
			for (size_t j = 0; j < nrOfweights; j++)
				vertices[i].weights[j] = vertices[i].weights[j] + ((vertices[i].weights[0] / prevPer)*missPer);
		}
	}
	Mesh* returnMesh = new Mesh(device, vertices, indices, textures);
	return returnMesh;
}

std::vector<Texture> Model::loadTextures(aiMaterial* material, aiTextureType textureType, std::string typeName)
{
	//Load texture
	std::vector<Texture> textures;

	for (size_t i = 0; i < material->GetTextureCount(textureType); i++)
	{
		aiString str;
		material->GetTexture(textureType, (UINT)i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (size_t j = 0; j < _loadedTextures.size(); j++)
		{
			if (std::strcmp(_loadedTextures[j]._path.c_str(), str.C_Str()) == 0)
			{
				textures.push_back(_loadedTextures[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			HRESULT hr;
			Texture texture;
			if (texType == "embedded compressed texture")
			{
				int textureIndex = getTextureIndex(&str);
				texture._texture = getTextureFromModel(textureIndex);
			}
			else
			{
				std::string filename = std::string(str.C_Str()); //Some string magic
				filename = _directory + '\\' + filename;
				std::wstring filenamews = std::wstring(filename.begin(), filename.end());
				hr = CoInitialize(NULL);
				hr = DirectX::CreateWICTextureFromFile(_device, filenamews.c_str(), NULL, &texture._texture);
				if (FAILED(hr))
					MessageBox(NULL, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
			}
			texture._type = typeName;
			texture._path = str.C_Str();
			textures.push_back(texture);
			_loadedTextures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}

	return textures;
}

std::string Model::determineTextureType(aiMaterial * material)
{
	aiString textureTypeStr;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &textureTypeStr);
	std::string textypeteststr = textureTypeStr.C_Str();
	if (textypeteststr == "*0" || textypeteststr == "*1" || textypeteststr == "*2" || textypeteststr == "*3" || textypeteststr == "*4" || textypeteststr == "*5")
	{
		if (_scene->mTextures[0]->mHeight == 0)
		{
			return "embedded compressed texture";
		}
		else
		{
			return "embedded non-compressed texture";
		}
	}
	if (textypeteststr.find('.') != std::string::npos)
	{
		return "textures are on disk";
	}
	
	return NULL;
}

int Model::getTextureIndex(aiString * str)
{
	//Kopierat fr�n Assimp doc...
	std::string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return stoi(tistr);
}

ID3D11ShaderResourceView * Model::getTextureFromModel(int textureIndex)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture;

	int* size = reinterpret_cast<int*>(&_scene->mTextures[textureIndex]->mWidth);

	hr = CreateWICTextureFromMemory(_device, _deviceContext, reinterpret_cast<unsigned char*>(_scene->mTextures[textureIndex]->pcData), *size, nullptr, &texture);
	if (FAILED(hr))
		MessageBox(NULL, L"Texture couldn't be created from memory!", L"Error!", MB_ICONERROR | MB_OK);

	return texture;
}

Model::Model()
{
}

Model::~Model()
{
	if (_transformationBuffer)
		delete _transformationBuffer;

	for (size_t i = 0; i < _meshes.size(); i++)
		delete _meshes[i];

	if (_boundingVolume)
		delete _boundingVolume;

	for (int i = 0; i < _loadedTextures.size(); i++)
		_loadedTextures[i]._texture->Release();
}

BoundingVolume * Model::getBoundingVolume()
{
	return _boundingVolume;
}

void Model::updateTransformation(DirectX::XMFLOAT3 position)
{
	_scene->mRootNode->mTransformation = aiMatrix4x4(aiVector3D(1, 1, 1), aiQuaternion(0, 0, 0), aiVector3D(position.x, position.y, position.z));

	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	_deviceContext->Map(*_transformationBuffer->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &_scene->mRootNode->mTransformation, sizeof(_scene->mRootNode->mTransformation));
	_deviceContext->Unmap(*_transformationBuffer->getConstantBuffer(), 0);
}

bool Model::loadModel(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string filename)
{
	//Load model from file
	_scene = _importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded); // aiProcessPreset_TargetRealtime_Quality kanske denna f�r optimisering

	if (_scene == NULL)
		return false;

	//Save member variables
	_directory = filename.substr(0, filename.find_last_of('\\')); //Tror inte det ska vara '/'
	_device = device;
	_deviceContext = deviceContext;
	
	//Start processing all the nodes in the model
	processNode(device, _scene->mRootNode);

	_transformationBuffer = new ConstantBuffer(_device, &_scene->mRootNode->mTransformation, sizeof(_scene->mRootNode->mTransformation));

	return true;
}

//void Model::animate(float timeInSec)
//{
//	//_scene->mRootNode->mTransformation = aiMatrix4x4(aiVector3D(1, 1, 1), aiQuaternion(0, 0, 0), aiVector3D(0, timeInSec, 0));
//
//	//D3D11_MAPPED_SUBRESOURCE mappedMemory;
//	//_deviceContext->Map(_transformationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
//	//memcpy(mappedMemory.pData, &_scene->mRootNode->mTransformation, sizeof(_scene->mRootNode->mTransformation));
//	//_deviceContext->Unmap(_transformationBuffer, 0);
//
//	//if (_scene->HasAnimations())
//	//{
//
//	//}
//	//const aiAnimation* anim = _scene->mAnimations[0];
//	//double currentTime = fmod(timeInSec * 1000, anim->mDuration);
//	//for (size_t i = 0; i < anim->mNumChannels; ++i)
//	//{
//	//	const aiNodeAnim* channel = anim->mChannels[i];
//	//	aiVector3D curPosition;
//	//	aiQuaternion curRotation;
//	//	// scaling purposefully left out 
//	//	// find the node which the channel affects
//	//	aiNode* targetNode = findNodeRecursivelyByName(_scene->mRootNode, channel->mNodeName);
//	//	// find current position
//	//	size_t posIndex = 0;
//	//	while (1)
//	//	{
//	//		// break if this is the last key - there are no more keys after this one, we need to use it
//	//		if (posIndex + 1 >= channel->mNumPositionKeys)
//	//			break;
//	//		// break if the next key lies in the future - the current one is the correct one then
//	//		if (channel->mPositionKeys[posIndex + 1].mTime > currentTime)
//	//			break;
//	//		posIndex++;
//	//	}
//	//	// maybe add a check here if the anim has any position keys at all
//	//	curPosition = channel->mPositionKeys[posIndex].mValue;
//	//	// same goes for rotation, but I shorten it now
//	//	size_t rotIndex = 0;
//	//	while (1)
//	//	{
//	//		if (rotIndex + 1 >= channel->mNumRotationKeys)
//	//			break;
//	//		if (channel->mRotationKeys[rotIndex + 1].mTime > currentTime)
//	//			break;
//	//		rotIndex++;
//	//	}
//	//	curRotation = channel->mRotationKeys[posIndex].mValue;
//	//	// now build a transformation matrix from it. First rotation, thenn push position in it as well. 
//	//	aiMatrix4x4 trafo = aiMatrix4x4(curRotation.GetMatrix());
//	//	trafo.a4 = curPosition.x; trafo.b4 = curPosition.y; trafo.c4 = curPosition.z;
//	//	// assign this transformation to the node
//	//	targetNode->mTransformation = trafo;
//	//}
//}

void Model::draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* transformationBuffer)
{
	for (int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i]->draw(deviceContext, *_transformationBuffer->getConstantBuffer());
	}
}
void Model::draw(ID3D11DeviceContext* deviceContext)
{
	for (int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i]->draw(deviceContext, *_transformationBuffer->getConstantBuffer());
	}
}

void Model::drawBoundingVolume(ID3D11DeviceContext * deviceContext)
{
	_boundingVolume->draw(deviceContext, *_transformationBuffer->getConstantBuffer());
}

void Model::processHeightmap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, size_t terrainWidth, size_t terrainHeight, std::vector<DirectX::XMFLOAT3> heightmap)
{
	_device = device;
	_deviceContext = deviceContext;

	// Used following tutorial: https://www.braynzarsoft.net/viewtutorial/q16390-30-heightmap-terrain
	// Why ++i instead of i++?: https://stackoverflow.com/questions/4706199/post-increment-and-pre-increment-within-a-for-loop-produce-same-output

	// TERRAIN (CREATING GRID) //
	//***********
	// LOAD HEIGHTMAP TO VERTICES //
	std::vector<Vertex> vertices(terrainWidth * terrainHeight);
	for (size_t i = 0; i < terrainWidth; ++i)
	{
		for (size_t j = 0; j < terrainHeight; ++j)
		{
			vertices[(i * (int)terrainWidth) + j]._position.x = heightmap[(i * (int)terrainWidth) + j].x;
			vertices[(i * (int)terrainWidth) + j]._position.y = heightmap[(i * (int)terrainWidth) + j].y;
			vertices[(i * (int)terrainWidth) + j]._position.z = heightmap[(i * (int)terrainWidth) + j].z;

			//NORMALS
			vertices[(i * (int)terrainWidth) + j]._normal.x = 0.0f;
			vertices[(i * (int)terrainWidth) + j]._normal.y = 1.0f;
			vertices[(i * (int)terrainWidth) + j]._normal.z = 0.0f;
		}
	}

	// INDICES AND TEXTURE COORDINATES
	int k = 0;
	int texUIndex = 0;
	int texVIndex = 0;
	int nrOfFaces = ((int)terrainWidth - 1)*((int)terrainHeight - 1) * 2;
	std::vector<int> indices(nrOfFaces * 3);

	for (size_t i = 0; i < (int)terrainWidth - 1; ++i)
	{
		for (size_t j = 0; j < (int)terrainHeight - 1; ++j)
		{
			indices[k + 5] = (int)(i * terrainHeight + j);        // Bottom left of quad
			vertices[i*terrainHeight + j]._textureCoords.x = texUIndex + 0.0f;
			vertices[i*terrainHeight + j]._textureCoords.y = texVIndex + 1.0f;

			indices[k + 4] = (int)(i * terrainHeight + j + 1);        // Bottom right of quad
			vertices[i*terrainHeight + j + 1]._textureCoords.x = texUIndex + 1.0f;
			vertices[i*terrainHeight + j + 1]._textureCoords.y = texVIndex + 1.0f;

			indices[k + 3] = (int)((i + 1)*terrainHeight + j);    // Top left of quad
			vertices[(i + 1)*terrainHeight + j]._textureCoords.x = texUIndex + 0.0f;
			vertices[(i + 1)*terrainHeight + j]._textureCoords.y = texVIndex + 0.0f;


			indices[k + 2] = (int)((i + 1)*terrainHeight + j);    // Top left of quad
			vertices[(i + 1)*terrainHeight + j]._textureCoords.x = texUIndex + 0.0f;
			vertices[(i + 1)*terrainHeight + j]._textureCoords.y = texVIndex + 0.0f;

			indices[k + 1] = (int)(i * terrainHeight + j + 1);        // Bottom right of quad
			vertices[i*terrainHeight + j + 1]._textureCoords.x = texUIndex + 1.0f;
			vertices[i*terrainHeight + j + 1]._textureCoords.y = texVIndex + 1.0f;

			indices[k] = (int)((i + 1)*terrainHeight + j + 1);    // Top right of quad
			vertices[(i + 1)*terrainHeight + j + 1]._textureCoords.x = texUIndex + 1.0f;
			vertices[(i + 1)*terrainHeight + j + 1]._textureCoords.y = texVIndex + 0.0f;

			k += 6; // next quad
			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}
	
	// TEXTURE(S)
	std::vector<Texture> textures;
	Texture texture;
	std::string filename = ".\\Resources\\Models\\grass.jpg";
	std::wstring filenameWS = std::wstring(filename.begin(), filename.end());
	
	HRESULT hr = CoInitialize(NULL);
	hr = DirectX::CreateWICTextureFromFile(_device, filenameWS.c_str(), NULL, &texture._texture);
	if (FAILED(hr))
		MessageBox(NULL, L"Texture couldn't be loader in function processHeightmap", L"Error!", MB_ICONERROR | MB_OK);
	

	textures.push_back(texture);
	_loadedTextures.push_back(texture);


	// BOUNDING VOLUME
	_boundingVolume = new OBB(_device, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	// MESH
	_meshes.push_back(new Mesh(_device, vertices, indices, textures)); // GLENN



	DirectX::XMMATRIX id = DirectX::XMMatrixIdentity();

	_transformationBuffer = new ConstantBuffer(device, &id, sizeof(id));

	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	_deviceContext->Map(*_transformationBuffer->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &id, sizeof(id));
	_deviceContext->Unmap(*_transformationBuffer->getConstantBuffer(), 0);
}
