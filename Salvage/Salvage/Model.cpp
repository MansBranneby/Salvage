#include "Model.h"
#include "Model.h"
#include <iostream>

void Model::boneTransform(float timeInSec, std::vector<aiMatrix4x4>& transforms)
{
	aiMatrix4x4 identityMatrix;
	// if animation has ticksPerSeconds that's not 0.0f pick that else pick 25.0f 
	float ticksPerSecond = _scene->mAnimations[0]->mTicksPerSecond != 0.0f ? (float)_scene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float timeInTicks = timeInSec * ticksPerSecond;
	float animationTime = fmod(timeInTicks, (float)_scene->mAnimations[0]->mDuration);

	readNodeHierachy(animationTime, _scene->mRootNode, identityMatrix);

	transforms.resize(_nrOfBones);

	for (UINT i = 0; i < _nrOfBones; ++i)
		transforms[i] = _boneInfo[i].finalTransformation;
}

void Model::readNodeHierachy(float animationTime, const aiNode* node, const aiMatrix4x4& parentTransform)
{
	std::string nodeName = node->mName.data;
	const aiAnimation* animation = _scene->mAnimations[0];
	aiMatrix4x4 nodeTransform = node->mTransformation;
	const aiNodeAnim* nodeAnimation = findNodeAnimation(animation, nodeName);

	if (nodeAnimation != nullptr)
	{
		// Scaling
		aiVector3D scaling = calculateInterporlatedScaling(animationTime, nodeAnimation);
		aiMatrix4x4 scalingMatrix;
		aiMatrix4x4::Scaling(scaling, scalingMatrix);

		// Rotation
		aiQuaternion rotation = calculateInterporlatedRotation(animationTime, nodeAnimation);
		aiMatrix4x4 rotationMatrix = aiMatrix4x4(rotation.GetMatrix());

		// Translation
		aiVector3D translation = calculateInterpolatedPosition(animationTime, nodeAnimation);
		aiMatrix4x4 translationMatrix;
		aiMatrix4x4::Translation(translation, translationMatrix);

		// Combine above matrices
		nodeTransform = translationMatrix * rotationMatrix * scalingMatrix;
	}

	aiMatrix4x4 globalInverse = _scene->mRootNode->mTransformation;
	globalInverse.Inverse();
	aiMatrix4x4 globalTransform = parentTransform * nodeTransform;

		if (_boneMapping.find(nodeName) != _boneMapping.end())
		{
			UINT boneIndex = _boneMapping[nodeName];
			_boneInfo[boneIndex].finalTransformation = globalInverse * globalTransform * _boneInfo[boneIndex].boneOffset;
		}

		for (UINT i = 0; i < node->mNumChildren; ++i)
			readNodeHierachy(animationTime, node->mChildren[i], globalTransform);
}

aiNodeAnim * Model::findNodeAnimation(const aiAnimation * animation, const std::string nodeName) const
{
	for (UINT i = 0; i < animation->mNumChannels; ++i)
	{
		if (animation->mChannels[i]->mNodeName.data == nodeName)
			return animation->mChannels[i];
	}

	return nullptr;
}

aiVector3D Model::calculateInterporlatedScaling(float animationTime, const aiNodeAnim * nodeAnimation)
{
	// If there's only one scaling key we will return that since we can't interpolate 1 value
	if (nodeAnimation->mNumScalingKeys == 1)
		return nodeAnimation->mScalingKeys[0].mValue;

	UINT scalingIndex = findScaling(animationTime, nodeAnimation);
	UINT nextScalingIndex = scalingIndex + 1;

	float deltaTime = (float)(nodeAnimation->mScalingKeys[nextScalingIndex].mTime - nodeAnimation->mScalingKeys[scalingIndex].mTime);
	float factor = (animationTime - (float)nodeAnimation->mScalingKeys[scalingIndex].mTime) / deltaTime;

	aiVector3D start = nodeAnimation->mScalingKeys[scalingIndex].mValue;
	aiVector3D end = nodeAnimation->mScalingKeys[nextScalingIndex].mValue;
	aiVector3D delta = end - start;
	
	return start + factor * delta;
}

aiQuaternion Model::calculateInterporlatedRotation(float animationTime, const aiNodeAnim * nodeAnimation)
{
	if (nodeAnimation->mNumRotationKeys == 1)
		return nodeAnimation->mRotationKeys[0].mValue;

	UINT rotationIndex = findRotation(animationTime, nodeAnimation);
	UINT nextRotationIndex = rotationIndex + 1;
	float deltaTime  = (float)(nodeAnimation->mRotationKeys[nextRotationIndex].mTime - nodeAnimation->mRotationKeys[rotationIndex].mTime);
	float factor = (animationTime - (float)nodeAnimation->mRotationKeys[rotationIndex].mTime) / deltaTime;

	const aiQuaternion& startRotation = nodeAnimation->mRotationKeys[rotationIndex].mValue;
	const aiQuaternion& endRotation = nodeAnimation->mRotationKeys[nextRotationIndex].mValue;
	
	aiQuaternion finalRotation;
	aiQuaternion::Interpolate(finalRotation, startRotation, endRotation, factor);

	return 	finalRotation.Normalize();
}

aiVector3D Model::calculateInterpolatedPosition(float animationTime, const aiNodeAnim * nodeAnimation)
{
	if (nodeAnimation->mNumPositionKeys == 1)
		return nodeAnimation->mPositionKeys[0].mValue;

	UINT positionIndex = findPosition(animationTime, nodeAnimation);
	UINT nextPositionIndex = positionIndex + 1;

	float deltaTime = (float)(nodeAnimation->mPositionKeys[nextPositionIndex].mTime - nodeAnimation->mPositionKeys[positionIndex].mTime);
	float factor = (animationTime - (float)nodeAnimation->mPositionKeys[positionIndex].mTime) / deltaTime;

	aiVector3D start = nodeAnimation->mPositionKeys[positionIndex].mValue;
	aiVector3D end = nodeAnimation->mPositionKeys[nextPositionIndex].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

UINT Model::findScaling(float animationTime, const aiNodeAnim * nodeAnimation)
{
	for (UINT i = 0; i < nodeAnimation->mNumScalingKeys - 1; ++i)
	{
		if (animationTime < (float)nodeAnimation->mScalingKeys[i + 1].mTime)
			return i;
	}

	assert(0);
	return 0;
}

UINT Model::findRotation(float animationTime, const aiNodeAnim * nodeAnimation)
{
	for (UINT i = 0; i < nodeAnimation->mNumRotationKeys - 1; ++i)
	{
		if (animationTime < (float)nodeAnimation->mRotationKeys[i + 1].mTime)
			return i;
	}

	assert(0);
	return 0;
}

UINT Model::findPosition(float animationTime, const aiNodeAnim * nodeAnimation)
{
	for (UINT i = 0; i < nodeAnimation->mNumPositionKeys - 1; ++i)
	{
		if (animationTime < (float)nodeAnimation->mPositionKeys[i + 1].mTime)
			return i;
	}

	assert(0);
	return 0;
}

void Model::processNode(ID3D11Device* device, aiNode * node)
{
	//// Cycle all meshes for current node
	//for (size_t i = 0; i < node->mNumMeshes; i++)
	//{
	//	aiMesh* tempAiMesh = _scene->mMeshes[node->mMeshes[i]];
	//	_meshes.push_back(this->processMesh(device, tempAiMesh));
	//}

	//// Go down tree and process all nodes
	//for (size_t i = 0; i < node->mNumChildren; i++)
	//	this->processNode(device, node->mChildren[i]);


	// Glenn: I have a tough time understanding nodes
	// so i'm going with this instead
	for (size_t i = 0; i < _scene->mNumMeshes; i++)
	{
		aiMesh* tempAiMesh = _scene->mMeshes[i];
		_meshes.push_back(processMesh(device, tempAiMesh));
	}
}

std::string texType;

Mesh* Model::processMesh(ID3D11Device* device, aiMesh * mesh)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::vector<Texture> textures;


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
	for (size_t i = 0; i < mesh->mNumVertices; ++i)
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
			vertex._textureCoords.x = (float)mesh->mTextureCoords[0][i].x; //Jag antar att diffuse texture ligger på index 0.
			vertex._textureCoords.x = (float)mesh->mTextureCoords[0][i].y; //Så när vi vill ha normal map ligger den troligen på ett annat index.
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
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			index.clear();
			weights.clear();
			//Loop through each bone
			for (size_t j = 0; j < mesh->mNumBones; ++j)
			{
				for (size_t k = 0; k < mesh->mBones[j]->mNumWeights; ++k)
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
			for (size_t j = 0; j < 4 && weights.size() != 0; ++j) //Här kan det nog krascha om en vertex har färre än 4 weights
			{
				int maxIndex = -1;
				float maxValue = -1;
				int eraseIndex = -1;
				for (size_t k = 0; k < weights.size(); ++k)
				{
					if (weights[k] > maxValue)
					{
						maxValue = weights[k];
						maxIndex = index[k];
						eraseIndex = (int)k;
					}
				}
				//Insert vertex boneinfo
				vertices[i].boneIndices[j] = maxIndex;
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

	// load bones
	// GLENN
	// get bone name
	for (UINT i = 0; i < mesh->mNumBones; ++i)
	{
		std::string boneName = mesh->mBones[i]->mName.data;
		UINT boneIndex = 0;
		if (_boneMapping.find(boneName) == _boneMapping.end())
		{
			boneIndex = _nrOfBones++;
			BoneInfo boneInfo;
			_boneInfo.push_back(boneInfo);
			_boneInfo[boneIndex].boneOffset = mesh->mBones[i]->mOffsetMatrix;
			_boneMapping[boneName] = boneIndex;
		}
		else
		{
			boneIndex = _boneMapping[boneName];
		}
	}


	Mesh* returnMesh = new ObjectMesh(device, vertices, indices, textures);
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
				filename = _fileDirectory + '\\' + filename;
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
	//Kopierat från Assimp doc...
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
	for (size_t i = 0; i < _meshes.size(); i++)
		delete _meshes[i];

	if (_boundingVolume)
		delete _boundingVolume;

	for (int i = 0; i < _loadedTextures.size(); i++)
		_loadedTextures[i]._texture->Release();
}


bool Model::loadModel(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string filename)
{
	// Load model from file
	_scene = _importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded); // aiProcessPreset_TargetRealtime_Quality kanske denna för optimisering

	// Failed import
	if (_scene == nullptr)
	{
		MessageBox(NULL, L"loadModel inside model.cpp", L"Error!", MB_ICONERROR | MB_OK);
		return false;
	}

	// Save member variables
	_fileDirectory = filename.substr(0, filename.find_last_of('\\'));
	_device = device;
	_deviceContext = deviceContext;

	// Start processing all the nodes in the model
	processNode(device, _scene->mRootNode);

	return true;
}

void Model::updateTransformation(DirectX::XMFLOAT3 position)
{
	_scene->mRootNode->mTransformation = aiMatrix4x4(aiVector3D(1, 1, 1), aiQuaternion(0, 0, 0), aiVector3D(position.x, position.y, position.z));
	_world = _scene->mRootNode->mTransformation;
}


void Model::animate(float timeInSec)
{
	if (_scene->HasAnimations())
	{
		// if animation has ticksPerSeconds that's not 0.0f pick that else pick 25.0f 
		float ticksPerSecond = _scene->mAnimations[0]->mTicksPerSecond != 0.0f ? (float)_scene->mAnimations[0]->mTicksPerSecond : 25.0f;
		float timeInTicks = timeInSec * ticksPerSecond;
		float AnimationTime = fmod(timeInTicks, (float)_scene->mAnimations[0]->mDuration);

		for (size_t i = 0; i < _scene->mAnimations[0]->mNumChannels; ++i)
		{
			const aiNodeAnim* channel = _scene->mAnimations[0]->mChannels[i];
			aiVector3D curPosition;
			aiQuaternion curRotation;
			// scaling purposefully left out 
			// find the node which the channel affects
			aiNode* targetNode = _scene->mRootNode->FindNode(channel->mNodeName);
			
			// find current position
			size_t posIndex = 0;
			while (1)
			{
				// break if this is the last key - there are no more keys after this one, we need to use it
				if (posIndex + 1 >= channel->mNumPositionKeys)
					break;
				// break if the next key lies in the future - the current one is the correct one then
				if (channel->mPositionKeys[posIndex + 1].mTime > AnimationTime)
					break;
				posIndex++;
			}
			// maybe add a check here if the anim has any position keys at all
			curPosition = channel->mPositionKeys[posIndex].mValue;
			// same goes for rotation, but I shorten it now
			size_t rotIndex = 0;
			while (1)
			{
				if (rotIndex + 1 >= channel->mNumRotationKeys)
					break;
				if (channel->mRotationKeys[rotIndex + 1].mTime > AnimationTime)
					break;
				rotIndex++;
			}
			curRotation = channel->mRotationKeys[posIndex].mValue;
			// now build a transformation matrix from it. First rotation, thenn push position in it as well. 
			aiMatrix4x4 trafo = aiMatrix4x4(curRotation.GetMatrix());
			trafo.a4 = curPosition.x; trafo.b4 = curPosition.y; trafo.c4 = curPosition.z;
			// assign this transformation to the node
			targetNode->mTransformation = trafo;
		}
	}
}

void Model::draw(GraphicResources * graphicResources)
{
	if (_scene != nullptr && _scene->HasAnimations() == true)
	{
		std::vector<aiMatrix4x4> transforms;
		boneTransform(0.0f, transforms);

		for (int i = 0; i < transforms.size(); ++i)
			graphicResources->getPerObjectData()->boneMatrices[i] = transforms[i];

		graphicResources->getPerObjectData()->hasAnimation = true;
	}
	else
	{
		graphicResources->getPerObjectData()->hasAnimation = false;
	}

	graphicResources->getPerObjectData()->world = _world;
	graphicResources->updatePerObjectCB();

	for (int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i]->draw(graphicResources);
	}
}

void Model::draw(GraphicResources* graphicResources, float timeInSec)
{
	if (_scene != nullptr && _scene->HasAnimations() == true)
	{
		std::vector<aiMatrix4x4> transforms;
		boneTransform(timeInSec, transforms);

		for(int i = 0; i < transforms.size(); ++i)
			graphicResources->getPerObjectData()->boneMatrices[i] = transforms[i];

		graphicResources->getPerObjectData()->hasAnimation = true;
	}
	else
	{
		graphicResources->getPerObjectData()->hasAnimation = false;
	}

	graphicResources->getPerObjectData()->world = _world;
	graphicResources->updatePerObjectCB();

	for (int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i]->draw(graphicResources);
	}
}

void Model::drawBoundingVolume(GraphicResources* graphicResources)
{
	graphicResources->getPerObjectData()->world = _world;
	graphicResources->updatePerObjectCB();

	_boundingVolume->draw(graphicResources);
}

std::vector<Mesh*>* Model::getMeshes()
{
	return &_meshes;
}

std::vector<Texture>* Model::getLoadedTextures()
{
	return &_loadedTextures;
}

BoundingVolume * Model::getBoundingVolume()
{
	return _boundingVolume;
}