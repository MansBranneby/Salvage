#include "Terrain.h"

void Terrain::loadHeightmap()
{
	FILE* filePtr;
	BITMAPFILEHEADER bitmapFileHeader;        //Structure which stores information about file
	BITMAPINFOHEADER bitmapInfoHeader;        //Structure which stores information about image

	size_t imageSize;
	if (fopen_s(&filePtr, _filename.c_str(), "rb") == 0)
	{
		fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

		//Get width and height of image
		_terrainWidth = bitmapInfoHeader.biWidth;
		_terrainHeight = bitmapInfoHeader.biHeight;


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
		unsigned char height;
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
		}

		delete [] bitmapImage;
		bitmapImage = 0;
	}

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
	loadTerrainModel(device, deviceContext, _terrainWidth, _terrainHeight, _heightmap); 
}

Terrain::~Terrain()
{
}

float Terrain::getHeight(float worldX, float worldZ)
{
	// Convert world space -> terrain space
	DirectX::XMMATRIX inverseModel = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixIdentity());
	DirectX::XMVECTOR terrainSpace = DirectX::XMVector4Transform(DirectX::XMVECTOR{ worldX, 0.0f, worldZ, 1.0f }, inverseModel);
	int x = (int)DirectX::XMVectorGetX(terrainSpace);
	int z = (int)DirectX::XMVectorGetZ(terrainSpace);

	if (x < 0 || z < 0 || x > (_terrainWidth - 1) || z > (_terrainHeight - 1))
		return 0.0f;

	// location in array
	int index = z * _terrainHeight + x;

	return _heightmap[index].y;
}
