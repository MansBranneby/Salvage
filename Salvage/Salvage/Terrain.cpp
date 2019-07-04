#include "Terrain.h"

void Terrain::loadHeightMap()
{
	FILE* filePtr;
	BITMAPFILEHEADER bitmapFileHeader;        //Structure which stores information about file
	BITMAPINFOHEADER bitmapInfoHeader;        //Structure which stores information about image
	size_t imageSize, index;
	unsigned char height;

	if (fopen_s(&filePtr, _filename, "rb"))
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
		_vertices = new DirectX::XMFLOAT3[_terrainWidth * _terrainHeight];

		

		// We divide the height by this number to "water down" the terrains height, otherwise the terrain will
		// appear to be "spikey" and not so smooth.
		float heightFactor = 10.0f;
		// We use a greyscale image, so all 3 rgb values are the same, but we only need one for the height
		// So we use this counter to skip the next two components in the image data (we read R, then skip BG)
		int k = 0;

		for (size_t i = 0; i < _terrainHeight; i++)
		{
			for (size_t j = 0; j < _terrainWidth; j++)
			{
				height = bitmapImage[k];
				index = (_terrainHeight * i) + j;

				_vertices[index].x = (float)j;
				_vertices[index].y = (float)height / heightFactor;
				_vertices[index].z = (float)i;

				k += 3;
			}
		}

		delete bitmapImage;
		bitmapImage = nullptr;
	}

}

Terrain::Terrain(char * filename)
{
	_filename = filename;
	_terrainWidth = 0;
	_terrainHeight = 0;
	//Load height map
	loadHeightMap();
}
