#include "Terrain.h"

float Terrain::getWidth() const
{
	return (_terrainWidth - 1) * _cellSpacing;
}

float Terrain::getDepth() const
{
	return (_terrainDepth - 1) * _cellSpacing;
}

void Terrain::loadHeightmap()
{
	FILE* filePtr;
	BITMAPFILEHEADER bitmapFileHeader;        //Structure which stores information about file
	BITMAPINFOHEADER bitmapInfoHeader;        //Structure which stores information about image

	UINT imageSize;
	if (fopen_s(&filePtr, _filename.c_str(), "rb") == NULL)
	{
		fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

		//Get width and height of image
		_terrainWidth = bitmapInfoHeader.biWidth;
		_terrainDepth = bitmapInfoHeader.biHeight;

		// image size: example 513x513 pixels, each pixel has rgb values
		imageSize = _terrainWidth * _terrainDepth * 3;

		// array store image data
		unsigned char* bitmapImage = new unsigned char[imageSize];

		// Set the file pointer to the beginning of the image data
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		// Store image data in bitmapImage
		fread(bitmapImage, 1, imageSize, filePtr);
		
		//Close file
		fclose(filePtr);

		//store vertices for terrain
		_heightmap.resize(_terrainWidth * _terrainDepth);

		// We use a greyscale image, so all 3 rgb values are the same, but we only need one for the height
		// So we use this counter to skip the next two components in the image data (we read R, then skip BG)
		int k = 0;
		unsigned char height = 0;
		for (UINT i = 0; i < _terrainDepth; ++i)
		{
			for (UINT j = 0; j < _terrainWidth; ++j)
			{
				height = bitmapImage[k];
				_heightmap[i * _terrainWidth + j] = (float)height / _heightFactor;

				k += 3;
			}
			k++;
		}

		// Smoothing terrain, reduces sharp edges
		Smooth();

		delete [] bitmapImage;
		bitmapImage = nullptr;
	}

}

void Terrain::buildGrid(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Used following tutorial: https://www.braynzarsoft.net/viewtutorial/q16390-30-heightmap-terrain
	// Why ++i instead of i++?: https://stackoverflow.com/questions/4706199/post-increment-and-pre-increment-within-a-for-loop-produce-same-output

	// TERRAIN (CREATING GRID) //
	_nrOfPatchVertRows = (_terrainDepth - 1) / _nrOfCellsPerPatch;
	_nrOfPatchVertCols = (_terrainWidth - 1) / _nrOfCellsPerPatch;
	_nrOfPatchVertices = _nrOfPatchVertRows * _nrOfPatchVertCols;
	_nrOfPatchFaces = (_nrOfPatchVertRows - 1) * (_nrOfPatchVertCols - 1);

	float halfWidth = getWidth() * 0.5f;
	float halfDepth = getDepth() * 0.5f;
	float patchWidth = getWidth() / (_nrOfPatchVertCols - 1);
	float patchDepth = getDepth() / (_nrOfPatchVertRows - 1);
	float du = 1.0f / (_nrOfPatchVertCols - 1);
	float dv = 1.0f / (_nrOfPatchVertRows - 1);

	std::vector<TerrainVertex> vertices(_nrOfPatchVertices);
	for (UINT i = 0; i < _nrOfPatchVertRows; ++i)
	{
		float z = halfDepth - i * patchDepth;
		for (UINT j = 0; j < _nrOfPatchVertCols; ++j)
		{
			float x = -halfWidth + j * patchWidth;

			// Positions
			vertices[i * _nrOfPatchVertCols + j]._position = { x, 0.0f, z };

			// Texture coordinates
			vertices[i * _nrOfPatchVertCols + j]._textureCoords = { j * du, i*dv };
		}
	}

	// calc bounds


	//// BOUNDING VOLUME
	//// Store axis-aligned bounding box y-bounds in upper-left patch corner.
	//for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
	//{
	//	for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
	//	{
	//		UINT patchID = i * (mNumPatchVertCols - 1) + j;
	//		patchVertices[i*mNumPatchVertCols + j].BoundsY = mPatchBoundsY[patchID];
	//	}
	//}

	// INDICES
	std::vector<int> indices(_nrOfPatchFaces * 4);
	int k = 0;
	for (UINT i = 0; i < _nrOfPatchVertRows - 1; ++i)
	{
		for (UINT j = 0; j < _nrOfPatchVertCols - 1; ++j)
		{
			// TOP LEFT && TOP RIGHT
			indices[k] = i * _nrOfPatchVertCols + j;
			indices[k + 1] = i * _nrOfPatchVertCols + j + 1;

			// BOTTOM LEFT && BOTTOM RIGHT
			indices[k + 2] = (i+1) * _nrOfPatchVertCols + j;
			indices[k + 3] = (i+1) * _nrOfPatchVertCols + j + 1;

			// Next quad
			k += 4; 
		}
	}

	// TEXTURES
	std::vector<Texture> textures;
	textures.push_back(Texture(device, ".\\Resources\\Textures\\grass.jpg"));
	textures.push_back(Texture(device, ".\\Resources\\Textures\\rock.jpg"));
	textures.push_back(Texture(device, ".\\Resources\\Textures\\texture.png"));
	textures.push_back(Texture(device, ".\\Resources\\Textures\\ground.png"));
	textures.push_back(Texture(device, ".\\Resources\\Textures\\blendmap.png"));
	textures.push_back(Texture(_heightmapSRV));
	getModel()->getLoadedTextures()->push_back(textures[0]);
	getModel()->getLoadedTextures()->push_back(textures[1]);
	getModel()->getLoadedTextures()->push_back(textures[2]);
	getModel()->getLoadedTextures()->push_back(textures[3]);
	getModel()->getLoadedTextures()->push_back(textures[4]);
	getModel()->getLoadedTextures()->push_back(textures[5]);

	// Create a mesh with vertices, indices and textures computed in the code above
	getModel()->getMeshes()->push_back(new TerrainMesh(device, vertices, indices, textures));

	// Create transformationbuffer, this buffer is sent into vertex shader to transform the terrain, currently just identity matrix
	getModel()->setTransformationBuffer(new ConstantBuffer(device, &DirectX::XMMatrixIdentity(), sizeof(DirectX::XMMatrixIdentity())));
}

bool Terrain::inBounds(int i, int j)
{
	return i >= 0 && i < (int)(_terrainDepth - 1) &&
		   j >= 0 && j < (int)(_terrainWidth - 1);
}

float Terrain::average(int i, int j)
{
	// Sample neighboring 8 neighboring pixel heights, average them.
	// 
    // ------------------------------
	//| i-1, j-1 | i-1, j | i-1, j+1 |
	// ------------------------------
	//| i  , j-1 | i  , j | i  , j+1 |
	// ------------------------------
	//| i+1, j-1 | i+1, j | i+1, j+1 |
	// ------------------------------

	float sumOfHeights = 0; //sum of the sampled heights from neighbors
	float nrOfSamples = 0;  // number of neighbors that have been sampled

	for (int m = i - 1; m < i + 1; ++m)
	{
		for (int n = j - 1; n < j + 1; ++n)
		{
			if (inBounds(m, n))
			{
				sumOfHeights += _heightmap[m * _terrainWidth + n];
				++nrOfSamples;
			}
		}
	}
	return sumOfHeights / nrOfSamples; // Averaged height   
}

void Terrain::Smooth()
{
	// Smooth reduces sharp edges in terrain
	for (UINT i = 0; i < _terrainDepth; ++i)
	{
		for (UINT j = 0; j < _terrainWidth; ++j)
		{
			_heightmap[i * _terrainWidth + j] = average(i, j);
		}
	}
}

void Terrain::createSRV(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = _terrainWidth;
	desc.Height = _terrainDepth;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R16_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	// HALF is defined in xnamath.h, for storing 16-bit float.
	std::vector<DirectX::PackedVector::HALF> hmap(_heightmap.size());
	std::transform(_heightmap.begin(), _heightmap.end(), hmap.begin(), DirectX::PackedVector::XMConvertFloatToHalf);
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hmap[0];
	data.SysMemPitch = _terrainWidth * sizeof(DirectX::PackedVector::HALF);
	data.SysMemSlicePitch = 0;
	ID3D11Texture2D* hmapTex = 0;
	HRESULT hr = device->CreateTexture2D(&desc, &data, &hmapTex);
	if (FAILED(hr))
		MessageBox(NULL, L"ERROR SRV in Terrain.cpp", L"Error", MB_OK | MB_ICONERROR);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	hr = device->CreateShaderResourceView(hmapTex, &srvDesc, &_heightmapSRV);
	if (FAILED(hr))
		MessageBox(NULL, L"ERROR SRV in Terrain.cpp", L"Error", MB_OK | MB_ICONERROR);

	if(hmapTex)
		hmapTex->Release();
}

Terrain::Terrain(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition, std::string filename)
	:StaticObject(TERRAIN, startingPosition)
{
	_nrOfCellsPerPatch = 64;
	_cellSpacing = 1.0f;

	_filename = ".\\Resources\\HeightMaps\\" + filename;
	_terrainWidth = 0;
	_terrainDepth = 0;
	_heightFactor = 10.0f; //Decides how spikey the terrain will be
	//Load heightmap
	loadHeightmap();

	//create grid with heightmap
	createSRV(device);
	buildGrid(device, deviceContext); 
}

Terrain::~Terrain()
{
}

float Terrain::getHeight(float worldX, float worldZ)
{
	// Convert world space -> terrain space
	DirectX::XMMATRIX inverseWorldMatrix = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixIdentity());
	DirectX::XMVECTOR terrainSpace = DirectX::XMVector4Transform(DirectX::XMVECTOR{ worldX, 0.0f, worldZ, 1.0f }, inverseWorldMatrix);
	float x = DirectX::XMVectorGetX(terrainSpace);
	float z = DirectX::XMVectorGetZ(terrainSpace);

	float c = (x + 0.5f*getWidth()) / _cellSpacing;
	float d = (z - 0.5f*getDepth()) / -_cellSpacing;

	// Terrain space to cell space
	// Find out which row and column object is in
	int row    = (int)floorf(d);
	int column = (int)floorf(c);

	if (row < 0 || column < 0 || row >= (int)(_terrainWidth - 1) || column >= (int)(_terrainDepth - 1))
		return 0.0f;

	// Get heights of the vertices in the cell we're in (cell = quad)
	// TL = top left
	// TR = top right
	// BL = bottom left
	// BR = bottom right
	float TL = _heightmap[row * _terrainWidth + column];
	float TR = _heightmap[row * _terrainWidth + column + 1];
	float BL = _heightmap[(row + 1) * _terrainWidth + column];
	float BR = _heightmap[(row + 1) * _terrainWidth + column + 1];

	// s and t are the coordinates inside the quad we're in
	// s and t are both in the range of [0, 1] and start in the upper left corner of the cell, just like UV coordinates for textures
	float s = c - (float)column;
	float t = d - (float)row;

	// If s + t <= 1.0f we're in upper triangle, otherwise we're in the lower
	if (s + t <= 1.0f)
	{
		float uy = TR - TL;
		float vy = BL - TL;
		return TL + s * uy + t * vy;
	}
	else
	{
		float uy = BL - BR;
		float vy = TR - BR;
		return BR + (1.0f - s)*uy + (1.0f - t)*vy;
	}
}
