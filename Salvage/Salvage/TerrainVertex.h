#pragma once
#include <DirectXMath.h>

class TerrainVertex
{
public:
	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT2 _textureCoords;
	DirectX::XMFLOAT2 _boundsY;

	TerrainVertex();
	~TerrainVertex();
};