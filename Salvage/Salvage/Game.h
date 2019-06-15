#pragma once
#include "Player.h"
#include <Keyboard.h>

class Game
{
private:
	ID3D11Device* _device; //Hopefully the only place in GameSystem with render stuff.
	ID3D11DeviceContext* _deviceContext;
	Player* _player;
	
public:
	Game(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~Game();
	void update(DirectX::Keyboard::State kbState, float deltaSeconds);
	void draw();
};