#include "ViewFrustum.h"

ViewFrustum::ViewFrustum()
{
}

ViewFrustum::ViewFrustum(DirectX::XMMATRIX viewProjection)
{
	using namespace DirectX;
	
	// Extract planes from ViewProjection matrix
	_planes[0] = viewProjection.r[3] + viewProjection.r[0]; // Left
	_planes[1] = viewProjection.r[3] - viewProjection.r[0]; // Right
	_planes[2] = viewProjection.r[3] + viewProjection.r[1]; // Bottom
	_planes[3] = viewProjection.r[3] - viewProjection.r[1]; // Top
	_planes[4] = viewProjection.r[3] + viewProjection.r[2]; // Near
	_planes[5] = viewProjection.r[3] - viewProjection.r[2]; // Far

	// Normalize planes
	for (size_t i = 0; i < 6; i++)
		_planes[i] = XMPlaneNormalize(_planes[i]);
}

ViewFrustum::~ViewFrustum()
{
}

DirectX::XMVECTOR * ViewFrustum::getPlanes()
{
	return _planes;
}
