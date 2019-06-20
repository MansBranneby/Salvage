#pragma once
#include "GameObject.h"

class StaticObject : public GameObject
{
private:

public:
	StaticObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition, std::string modelFile);
	~StaticObject();

	void updateLogic(); //Otherwise abstract
};