#include "Robot.h"

Robot::Robot(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition)
	:DynamicObject(device, deviceContext, startingPosition)
{
}

Robot::~Robot()
{
}
