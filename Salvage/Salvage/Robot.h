#pragma once
#include "DynamicObject.h"

class Robot : public DynamicObject
{
private:
	DirectX::XMVECTOR _velocity;
	float _speed;
	
public:
	Robot(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~Robot() = 0;

	virtual DirectX::XMVECTOR getVelocity();
	float getSpeed();

	virtual void setVelocity(DirectX::XMVECTOR velocity);
	virtual void setSpeed(float speed);
};