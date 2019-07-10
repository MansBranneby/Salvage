#pragma once
#include "Robot.h"
#include <Keyboard.h>
#include <GamePad.h>

class Player : public Robot
{
private:
	
public:
	Player(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition, std::string modelFile);
	~Player();
	void abstract();
	void setHeight(float height);

	void handleInput(Keyboard::State kb, GamePad::State gp, float deltaSeconds);
};