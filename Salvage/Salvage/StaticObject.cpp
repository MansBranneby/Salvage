#include "StaticObject.h"

StaticObject::StaticObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition)
	:GameObject(device, deviceContext, STATICOBJECT, startingPosition)
{
}

StaticObject::~StaticObject()
{
}

void StaticObject::updateLogic()
{
}
