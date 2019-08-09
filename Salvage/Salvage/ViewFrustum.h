#pragma once
#include <DirectXMath.h>

class ViewFrustum
{
private:
	DirectX::XMVECTOR _planes[6];

public:
	ViewFrustum();
	ViewFrustum(DirectX::XMMATRIX viewProjection);
	~ViewFrustum();
	DirectX::XMVECTOR* getPlanes();
};