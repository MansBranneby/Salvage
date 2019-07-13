#pragma once
#include "GameObject.h"

class DynamicObject : public GameObject
{
public:
	DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~DynamicObject();

	void move(DirectX::XMVECTOR velocity);
	void updateLogic();
	virtual void abstract() = 0;
};