#pragma once
#include "GameObject.h"

class DynamicObject : public GameObject
{
private:
	DirectX::XMVECTOR _velocity;
public:
	DynamicObject();
	~DynamicObject();

	void updateLogic();
	virtual void abstract() = 0;
};