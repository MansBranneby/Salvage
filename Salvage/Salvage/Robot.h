#pragma once
#include "DynamicObject.h"

class Robot : public DynamicObject
{
private:

	
public:
	Robot(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~Robot() = 0;
};