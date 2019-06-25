#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:

public:
	OBB(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates);
};