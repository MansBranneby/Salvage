#pragma once
#include "Robot.h"
#include <Keyboard.h>
#include <GamePad.h>

class Player : public Robot
{
public:
	Player(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~Player();

	void handleInput(Keyboard::State kb, GamePad::State gp, float deltaSeconds);
};