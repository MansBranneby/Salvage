#pragma once
#include "DynamicObject.h"

class Robot : public DynamicObject
{
private:

public:
	Robot(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition);
	~Robot();
	virtual void abstract() = 0;
};