#include "StaticObject.h"

StaticObject::StaticObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, STATICOBJECT, startingPosition, modelFile)
{
}

StaticObject::~StaticObject()
{
}

void StaticObject::updateLogic()
{
}
