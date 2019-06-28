#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:
	////Center of OBB
	//DirectX::XMVECTOR _center;
	////Local axes
	//DirectX::XMVECTOR _xAxis;
	//DirectX::XMVECTOR _yAxis;
	//DirectX::XMVECTOR _zAxis;
	//Half width, height and depth
	DirectX::XMFLOAT3 _halfXYZ;

public:
	OBB(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates);
	virtual bool intersectsWithOBB(BoundingVolume* other);
};