#include "Game.h"

void Game::updateCamera()
{
	_camera->followObject(_player->getPosition());
}

void Game::updateCameraBuffers()
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	_deviceContext->Map(*_camera->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, _camera->getTransformMatrices(), sizeof(*_camera->getTransformMatrices()));
	_deviceContext->Unmap(*_camera->getConstantBuffer(), 0);

	_deviceContext->VSSetConstantBuffers(0, 1, _camera->getConstantBuffer());
}

Game::Game(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float width, float height)
{
	//Graphics
	_device = device;
	_deviceContext = deviceContext;

	//Camera
	_camera = new Camera(_device, width, height);

	//Spawn location of player
	DirectX::XMVECTOR startingPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); //Player och denna ska nog skapas och sättas i LevelAi för att sedan skicka en referens till Game. 
	_player = new Player(_device, _deviceContext, startingPosition);

	_staticObject = new Player(_device, _deviceContext, startingPosition);
}

Game::~Game()
{
	delete _camera;
	delete _player;
	delete _staticObject;
}

void Game::update(DirectX::Keyboard::State kb, float deltaSeconds)
{

	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	if (kb.W) //Forward
		_player->move(DirectX::XMFLOAT3(0.0f, 0.0f, deltaSeconds*100.0f)); //Update with player speed
	if (kb.S) //Backwards
		_player->move(DirectX::XMFLOAT3(0.0f, 0.0f, -deltaSeconds * 100.0f));
	if (kb.A)	//Left
		_player->move(DirectX::XMFLOAT3(-deltaSeconds * 100.0f, 0.0f, 0.0f));
	if (kb.D)	//Right
		_player->move(DirectX::XMFLOAT3(deltaSeconds * 100.0f, 0.0f, 0.0f));

	_player->updateLogic();
	
	//Update Camera
	updateCamera();
}

void Game::draw()
{
	//Först uppdatera kamerans ConstantBuffers
	updateCameraBuffers();

	//Senare gå igenom alla GameObject
	_player->draw(_deviceContext);
	_staticObject->draw(_deviceContext);
}