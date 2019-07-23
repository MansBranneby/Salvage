#include "Terrain.h"

void Terrain::loadHeightmap()
{
	FILE* filePtr;
	BITMAPFILEHEADER bitmapFileHeader;        //Structure which stores information about file
	BITMAPINFOHEADER bitmapInfoHeader;        //Structure which stores information about image

	size_t imageSize;
	if (fopen_s(&filePtr, _filename.c_str(), "rb") == NULL)
	{
		fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

		//Get width and height of image
		_terrainWidth = bitmapInfoHeader.biWidth;
		_terrainHeight = bitmapInfoHeader.biHeight;

		// image size: example 513x513 pixels, each pixel has rgb values
		imageSize = _terrainWidth * _terrainHeight * 3;

		// array store image data
		unsigned char* bitmapImage = new unsigned char[imageSize];

		// Set the file pointer to the beginning of the image data
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		// Store image data in bitmapImage
		fread(bitmapImage, 1, imageSize, filePtr);
		
		//Close file
		fclose(filePtr);

		//store vertices for terrain
		_heightmap.resize(_terrainWidth * _terrainHeight);

		// We use a greyscale image, so all 3 rgb values are the same, but we only need one for the height
		// So we use this counter to skip the next two components in the image data (we read R, then skip BG)
		int k = 0;
		unsigned char height = 0;
		size_t index;
		for (size_t i = 0; i < _terrainHeight; ++i)
		{
			for (size_t j = 0; j < _terrainWidth; ++j)
			{
				height = bitmapImage[k];

				index = (_terrainHeight * i) + j;

				_heightmap[index].x = (float)j;
				_heightmap[index].y = (float)height / _heightFactor;
				_heightmap[index].z = (float)i;

				k += 3;
			}
			k++;
		}

		delete [] bitmapImage;
		bitmapImage = nullptr;
	}

}

void Terrain::processHeightmap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, size_t terrainWidth, size_t terrainHeight, std::vector<DirectX::XMFLOAT3> heightmap)
{
	// Used following tutorial: https://www.braynzarsoft.net/viewtutorial/q16390-30-heightmap-terrain
	// Why ++i instead of i++?: https://stackoverflow.com/questions/4706199/post-increment-and-pre-increment-within-a-for-loop-produce-same-output

	// TERRAIN (CREATING GRID) //
	//***********
	// LOAD HEIGHTMAP AND CALCULATE NORMALS//
	XMVECTOR normal(XMVectorZero());
	std::vector<TerrainVertex> vertices(terrainWidth * terrainHeight);
	for (size_t i = 0; i < terrainWidth; ++i)
	{
		for (size_t j = 0; j < terrainHeight; ++j)
		{
			// POSITIONS
			vertices[i * terrainWidth + j]._position = heightmap[i * terrainWidth + j];

			// NORMALS
			// https://stackoverflow.com/questions/13983189/opengl-how-to-calculate-normals-in-a-terrain-height-grid
			// https://www.gamedev.net/forums/topic/163625-fast-way-to-calculate-heightmap-normals/
			float hL = getHeight((float)j - 1.0f, (float)i);
			float hR = getHeight((float)j + 1.0f, (float)i);
			float hD = getHeight((float)j, (float)i - 1.0f);
			float hU = getHeight((float)j, (float)i + 1.0f);

			normal = DirectX::XMVectorSet(hL - hR, 2.0f, hD - hU, 0.0f);
			normal = XMVector3Normalize(normal);

			XMStoreFloat3(&vertices[i * terrainWidth + j]._normal, normal);
		}
	}

	// INDICES AND TEXTURE COORDINATES
	int k = 0;
	int texUIndex = 0;
	int texVIndex = 0;
	size_t nrOfFaces = (terrainWidth - 1)*(terrainHeight - 1) * 2;
	std::vector<int> indices(nrOfFaces * 3);

	for (size_t i = 0; i < terrainWidth - 1; ++i)
	{
		for (size_t j = 0; j < (int)terrainHeight - 1; ++j)
		{
			indices[k + 5] = (int)(i * terrainHeight + j);        // Bottom left of quad
			vertices[i*terrainHeight + j]._textureCoords0.x = texUIndex + 0.0f;
			vertices[i*terrainHeight + j]._textureCoords0.y = texVIndex + 1.0f;

			indices[k + 4] = (int)(i * terrainHeight + j + 1);        // Bottom right of quad
			vertices[i*terrainHeight + j + 1]._textureCoords0.x = texUIndex + 1.0f;
			vertices[i*terrainHeight + j + 1]._textureCoords0.y = texVIndex + 1.0f;

			indices[k + 3] = (int)((i + 1)*terrainHeight + j);    // Top left of quad
			vertices[(i + 1)*terrainHeight + j]._textureCoords0.x = texUIndex + 0.0f;
			vertices[(i + 1)*terrainHeight + j]._textureCoords0.y = texVIndex + 0.0f;


			indices[k + 2] = (int)((i + 1)*terrainHeight + j);    // Top left of quad
			vertices[(i + 1)*terrainHeight + j]._textureCoords0.x = texUIndex + 0.0f;
			vertices[(i + 1)*terrainHeight + j]._textureCoords0.y = texVIndex + 0.0f;

			indices[k + 1] = (int)(i * terrainHeight + j + 1);        // Bottom right of quad
			vertices[i*terrainHeight + j + 1]._textureCoords0.x = texUIndex + 1.0f;
			vertices[i*terrainHeight + j + 1]._textureCoords0.y = texVIndex + 1.0f;

			indices[k] = (int)((i + 1)*terrainHeight + j + 1);    // Top right of quad
			vertices[(i + 1)*terrainHeight + j + 1]._textureCoords0.x = texUIndex + 1.0f;
			vertices[(i + 1)*terrainHeight + j + 1]._textureCoords0.y = texVIndex + 0.0f;

			k += 6; // next quad
			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}

	// TEXTURE(S)

	// GLENN
	std::vector<Texture> textures;
	textures.push_back(Texture(device, ".\\Resources\\Textures\\grass.jpg"));
	textures.push_back(Texture(device, ".\\Resources\\Textures\\purple.png"));
	textures.push_back(Texture(device, ".\\Resources\\Textures\\texture.png"));
	textures.push_back(Texture(device, ".\\Resources\\Textures\\ground.png"));
	getModel()->getLoadedTextures()->push_back(textures[0]);
	getModel()->getLoadedTextures()->push_back(textures[1]);
	getModel()->getLoadedTextures()->push_back(textures[2]);
	getModel()->getLoadedTextures()->push_back(textures[3]);

	// Create a mesh with vertices, indices and textures computed in the code above
	getModel()->getMeshes()->push_back(new TerrainMesh(device, vertices, indices, textures));

	// Create transformationbuffer, this buffer is sent into vertex shader to transform the terrain, currently just identity matrix
	getModel()->setTransformationBuffer(new ConstantBuffer(device, &DirectX::XMMatrixIdentity(), sizeof(DirectX::XMMatrixIdentity())));
}

Terrain::Terrain(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition, std::string filename)
	:StaticObject(TERRAIN, startingPosition)
{
	_filename = ".\\Resources\\HeightMaps\\" + filename;
	_terrainWidth = 0;
	_terrainHeight = 0;
	_heightFactor = 10.0f; //Decides how spikey the terrain will be
	//Load heightmap
	loadHeightmap();

	//function in GameObject
	//create grid with heightmap
	processHeightmap(device, deviceContext, _terrainWidth, _terrainHeight, _heightmap); 
}

Terrain::~Terrain()
{
}

float Terrain::getHeight(float worldX, float worldZ)
{
	// Convert world space -> terrain space
	DirectX::XMMATRIX inverseWorldMatrix = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixIdentity());
	DirectX::XMVECTOR terrainSpace = DirectX::XMVector4Transform(DirectX::XMVECTOR{ worldX, 0.0f, worldZ, 1.0f }, inverseWorldMatrix);
	int x = (int)DirectX::XMVectorGetX(terrainSpace);
	int z = (int)DirectX::XMVectorGetZ(terrainSpace);

	if (x < 0 || z < 0 || x > (_terrainWidth - 1) || z > (_terrainHeight - 1))
		return 0.0f;

	// location in array
	int index = z * (int)_terrainHeight + x;

	return _heightmap[index].y;
}
