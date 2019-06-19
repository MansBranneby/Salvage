#pragma once
#include "GameObject.h"

class DynamicObject : public GameObject
{
private:
	DirectX::XMFLOAT3 _velocity;
public:
	DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition);
	~DynamicObject();

	void move(DirectX::XMFLOAT3 speed);
	void updateLogic();
	virtual void abstract() = 0;
};