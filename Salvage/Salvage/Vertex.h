#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

class Vertex
{
public:
	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT3 _normal;
	DirectX::XMFLOAT2 _textureCoords;
	UINT boneIndices[4];
	float weights[4];

	Vertex();
	~Vertex();
};