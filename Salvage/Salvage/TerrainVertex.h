#pragma once
#include <DirectXMath.h>

class TerrainVertex
{
public:
	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT3 _normal;
	DirectX::XMFLOAT2 _textureCoords0;
	DirectX::XMFLOAT2 _textureCoords1;

	TerrainVertex();
	~TerrainVertex();
};