#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:
	// Local axes
	DirectX::XMVECTOR _xAxis;
	DirectX::XMVECTOR _yAxis;
	DirectX::XMVECTOR _zAxis;
	// Half with, height and length
	DirectX::XMFLOAT3 _halfXYZ;

public:
	OBB(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates);
	void move();
	virtual bool intersectsWithOBB(BoundingVolume* other);
};