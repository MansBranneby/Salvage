#include "DynamicObject.h"

DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, objType, startingPosition, modelFile)
{
	_speed = 25.0f;
	_velocity = { 0.0f, 0.0f, 0.0f };
}

DynamicObject::~DynamicObject()
{
}

DirectX::XMVECTOR DynamicObject::getVelocity()
{
	return _velocity;
}

float DynamicObject::getSpeed()
{
	return _speed;
}

void DynamicObject::setVelocity(DirectX::XMVECTOR velocity)
{
	_velocity = velocity;
}

void DynamicObject::setSpeed(float speed)
{
	_speed = speed;
}

void DynamicObject::move(DirectX::XMVECTOR velocity)
{
	_position += velocity;
	getBoundingVolume()->move(velocity);
}

void DynamicObject::updateLogic()
{
	updateTransformations(XMFLOAT3(XMVectorGetX(_position), XMVectorGetY(_position), XMVectorGetZ(_position)));
}
