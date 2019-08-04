#include "DynamicObject.h"

DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, objType, startingPosition, modelFile)
{
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::move(DirectX::XMVECTOR velocity)
{
	_position += velocity;
	getBoundingVolume()->move(velocity);

	updateTransformations(XMFLOAT3(XMVectorGetX(_position), XMVectorGetY(_position), XMVectorGetZ(_position)));
}
