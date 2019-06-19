#pragma once
#include "DynamicObject.h"

class Robot : public DynamicObject
{
private:
protected:
	float _speed;
public:
	Robot(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition);
	~Robot();
	virtual void abstract() = 0;

	float getSpeed();
};