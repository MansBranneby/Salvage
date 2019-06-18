#pragma once
#include "Player.h"
#include "Camera.h"
#include <Keyboard.h>

class Game
{
private:
	ID3D11Device* _device = nullptr; //Hopefully the only place in GameSystem with render stuff.
	ID3D11DeviceContext* _deviceContext = nullptr;
	Camera* _camera = nullptr;
	Player* _player = nullptr;
	GameObject* _staticObject; //test

	void updateCamera();
	void updateCameraBuffers();
	
public:
	Game(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float width, float height);
	~Game();
	void update(DirectX::Keyboard::State kbState, float deltaSeconds);
	void draw();
};