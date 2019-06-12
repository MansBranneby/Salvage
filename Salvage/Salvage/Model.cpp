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

std::string texType;

Mesh Model::processMesh(ID3D11Device* device, aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::vector<Texture> textures;

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		if (texType.empty())
			texType = determineTextureType(scene, material);
	}

	//Loop vertices
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		//Vertex
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;
		
		//Texturecoord
		if (mesh->mTextureCoords[0])
		{
			vertex.u = (float)mesh->mTextureCoords[0][i].x; //Jag antar att diffuse texture ligger p� index 0.
			vertex.v = (float)mesh->mTextureCoords[0][i].y; //S� n�r vi vill ha normal map ligger den troligen p� ett annat index.
		}

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
		std::vector<Texture> texture = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse",scene);
		textures.insert(textures.end(), texture.begin(), texture.end());
	}

	return Mesh(device, vertices, indices, textures);
}

std::vector<Texture> Model::loadTextures(aiMaterial* material, aiTextureType textureType, std::string typeName, const aiScene * scene)
{
	//Load texture
	std::vector<Texture> textures;

	for (int i = 0; i < material->GetTextureCount(textureType); i++)
	{
		aiString str;
		material->GetTexture(textureType, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (int j = 0; j < _loadedTextures.size(); j++)
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
				texture._texture = getTextureFromModel(scene, textureIndex);
			}
			else
			{
				std::string filename = std::string(str.C_Str()); //Some string magic
				filename = _directory + '\\' + filename; //Tror inte det ska vara '/'
				std::wstring filenamews = std::wstring(filename.begin(), filename.end());
				hr = CreateWICTextureFromFile(_device, _deviceContext, filenamews.c_str(), nullptr, &texture._texture);
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

std::string Model::determineTextureType(const aiScene * scene, aiMaterial * material)
{
	aiString textureTypeStr;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &textureTypeStr);
	std::string textypeteststr = textureTypeStr.C_Str();
	if (textypeteststr == "*0" || textypeteststr == "*1" || textypeteststr == "*2" || textypeteststr == "*3" || textypeteststr == "*4" || textypeteststr == "*5")
	{
		if (scene->mTextures[0]->mHeight == 0)
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
}

int Model::getTextureIndex(aiString * str)
{
	//Kopierat fr�n Assimp doc...
	std::string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return stoi(tistr);
}

ID3D11ShaderResourceView * Model::getTextureFromModel(const aiScene * scene, int textureIndex)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture;

	int* size = reinterpret_cast<int*>(&scene->mTextures[textureIndex]->mWidth);

	hr = CreateWICTextureFromMemory(_device, _deviceContext, reinterpret_cast<unsigned char*>(scene->mTextures[textureIndex]->pcData), *size, nullptr, &texture);
	if (FAILED(hr))
		MessageBox(NULL, L"Texture couldn't be created from memory!", L"Error!", MB_ICONERROR | MB_OK);

	return texture;
}

Model::Model()
{
}

Model::~Model()
{
}

bool Model::loadModel(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string filename)
{
	//Load model from file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (scene == NULL)
		return false;

	//Save member variables
	_directory = filename.substr(0, filename.find_last_of('\\')); //Tror inte det ska vara '/'
	_device = device;
	_deviceContext = deviceContext;

	//Start processing all the nodes in the model
	processNode(device, scene->mRootNode, scene);
}

void Model::draw(ID3D11DeviceContext* deviceContext)
{
	for (int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i].draw(deviceContext);
	}
}
