#include "Robot.h"

Robot::Robot(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:DynamicObject(device, deviceContext, objType, startingPosition, modelFile)
{
	_speed = 25.0f;
}

Robot::~Robot()
{
}

DirectX::XMVECTOR Robot::getVelocity()
{
	return _velocity;
}

float Robot::getSpeed()
{
	return _speed;
}

void Robot::setVelocity(DirectX::XMVECTOR velocity)
{
	_velocity = velocity;
}

void Robot::setSpeed(float speed)
{
	_speed = speed;
}
