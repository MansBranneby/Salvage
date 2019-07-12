#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

class Vertex
{
private:
public:
	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT3 _normal;
	DirectX::XMFLOAT2 _textureCoords;
	float weights[4];
	float boneIndices[4];

	Vertex();
	~Vertex();
};