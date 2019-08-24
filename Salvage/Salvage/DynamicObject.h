#pragma once
#include "GameObject.h"

class DynamicObject : public GameObject
{
private:
	DirectX::XMVECTOR _velocity;
	DirectX::XMVECTOR _acceleration;
	float _friction;

public:
	DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~DynamicObject();

	virtual void setVelocity(DirectX::XMVECTOR velocity);
	virtual void setAcceleration(DirectX::XMVECTOR acceleration);
	virtual DirectX::XMVECTOR getVelocity();
	virtual DirectX::XMVECTOR getAcceleration();
	
	void move(DirectX::XMVECTOR acceleration, float deltaSeconds);
};