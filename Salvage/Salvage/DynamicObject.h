#pragma once
#include "GameObject.h"

class DynamicObject : public GameObject
{
private:
	DirectX::XMVECTOR _velocity;
	float _speed;

public:
	DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile);
	~DynamicObject();

	virtual DirectX::XMVECTOR getVelocity();
	virtual float getSpeed();

	virtual void setVelocity(DirectX::XMVECTOR velocity);
	virtual void setSpeed(float speed);

	void move(DirectX::XMVECTOR velocity);
	void updateLogic();
	virtual void abstract() = 0;
};