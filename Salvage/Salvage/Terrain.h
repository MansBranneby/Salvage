#pragma once
#include "StaticObject.h"

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <string>
#include <iostream>
#include <Windows.h>

class Terrain : public StaticObject
{
private:
	std::string _filename;
	UINT _terrainWidth;
	UINT _terrainDepth;
	float _heightFactor;
	std::vector<float> _heightmap;
	ID3D11ShaderResourceView* _heightmapSRV;

	UINT _nrOfPatchVertRows;
	UINT _nrOfPatchVertCols;
	UINT _nrOfPatchVertices;
	UINT _nrOfPatchFaces;
	int _cellsPerPatch;
	float _cellSpacing;

	float getWidth() const;
	float getDepth() const;

	void loadHeightmap();
	void buildGrid(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	
	// Height bounds
	std::vector <DirectX::XMFLOAT2> _patchHeightBounds;
	void calculateAllHeightBounds();
	void calculateHeightBounds(UINT i, UINT j);

	// Smooth out terrain
	bool inBounds(int i, int j); 
	float average(int i, int j);
	void Smooth();

	// create SRV of heightmap
	void createSRV(ID3D11Device* device);
public:
	Terrain(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosititon, std::string filename);
	~Terrain();

	float getHeight(float worldX, float worldZ);
};