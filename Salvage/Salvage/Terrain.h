#pragma once
#include "DirectXMath.h"
#include <stdio.h>
#include <Windows.h>
class Terrain
{
private:
	char* _filename;
	size_t _terrainWidth;
	size_t _terrainHeight;
	DirectX::XMFLOAT3* _vertices;
	void loadHeightMap();

public:
	Terrain(char* filename);
};