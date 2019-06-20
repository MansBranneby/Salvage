#pragma once
#include "Player.h"
#include "Enemy.h"
#include "StaticObject.h"

class GameObjectHandler
{
private:
	//Graphics
	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _deviceContext = nullptr;

	//Container
	int _capacity;
	int _nrOfObjects;
	GameObject** _gameObjects = nullptr;

	//Manage container
	void freeMemory();
	void expand();
	void initiate(int from = 0);
public:
	GameObjectHandler(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int capacity = 10);
	virtual~GameObjectHandler();

	void addGameObject(ObjectType objType, XMVECTOR position, std::string modelFile);
	void drawObjects();
	GameObject* getGameObject(int index) const;
};