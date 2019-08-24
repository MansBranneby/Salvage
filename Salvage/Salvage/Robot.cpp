#include "Robot.h"

Robot::Robot(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:DynamicObject(device, deviceContext, objType, startingPosition, modelFile)
{
}

Robot::~Robot()
{
}