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
	UINT _terrainHeight;
	float _heightFactor;
	std::vector<DirectX::XMFLOAT3> _heightmap;
	ID3D11ShaderResourceView* _heightmapSRV; // used for tesselation

	void loadHeightmap();
	void processHeightmap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT terrainWidth, UINT terrainHeight, std::vector<DirectX::XMFLOAT3> heightmap);
	
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