#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:

public:
	OBB(DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates);
};