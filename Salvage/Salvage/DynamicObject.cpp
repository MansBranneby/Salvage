#include "DynamicObject.h"

DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, objType, startingPosition, modelFile)
{
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::move(DirectX::XMFLOAT3 speed)
{
	_lookAt = XMLoadFloat3(&speed);
	//_velocity = velocity;
	_position += XMVectorSet(speed.x, speed.y, speed.z, 0.0f);// * _lookAt;
	speed = XMFLOAT3(0.0f, 0.0f, 0.0f);


}

void DynamicObject::updateLogic()
{
	updateTransformations(XMFLOAT3(XMVectorGetX(_position), XMVectorGetY(_position), XMVectorGetZ(_position)));
}
