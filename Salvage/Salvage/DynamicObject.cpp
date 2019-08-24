#include "DynamicObject.h"

DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, objType, startingPosition, modelFile)
{
	_acceleration = { 0.0f, 0.0f, 0.0f, 0.0f };
	_velocity = { 0.0f, 0.0f, 0.0f, 0.0f };
	_friction = 0.000000001f;
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::move(DirectX::XMVECTOR acceleration, float deltaSeconds)
{
	_velocity += acceleration;
	_velocity *= pow(_friction, deltaSeconds);
	setPosition(getPositionVector() + _velocity);
    
	// Update transformation matrix
	updateTransformations(getPositionFloat3());
	getBoundingVolume()->move(acceleration);
}

void DynamicObject::setVelocity(DirectX::XMVECTOR velocity)
{
	_velocity = velocity;
}

void DynamicObject::setAcceleration(DirectX::XMVECTOR acceleration)
{
	_acceleration = acceleration;
}

DirectX::XMVECTOR DynamicObject::getVelocity()
{
	return _velocity;
}

DirectX::XMVECTOR DynamicObject::getAcceleration()
{
	return _acceleration;
}
