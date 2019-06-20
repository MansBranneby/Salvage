#pragma once
#include "Robot.h"

class Player : public Robot
{
private:
	
public:
	Player(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition, std::string modelFile);
	~Player();
	void abstract();
};