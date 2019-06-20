#include "Robot.h"

Robot::Robot(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:DynamicObject(device, deviceContext, objType, startingPosition, modelFile)
{
	_speed = 30;
}

Robot::~Robot()
{
}

float Robot::getSpeed()
{
	return _speed;
}
