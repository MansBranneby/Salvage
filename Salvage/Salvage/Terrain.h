#pragma once
#include "StaticObject.h"

#include <DirectXMath.h>
#include <string>
#include <iostream>
#include <Windows.h>

class Terrain : public StaticObject
{
private:
	std::string _filename;
	size_t _terrainWidth;
	size_t _terrainHeight;
	float _heightFactor;
	std::vector<DirectX::XMFLOAT3> _heightmap;
	void loadHeightmap();

public:
	Terrain(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosititon, std::string filename);
	~Terrain();

	float getHeight(float worldX, float worldZ);
};