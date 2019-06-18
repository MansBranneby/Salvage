#pragma once
#include "GameObject.h"

class GameObjectHandler
{
private:
	int _capacity;
	int _nrOfObjects;
	GameObject** _gameObjects = nullptr;

	void freeMemory();
	void expand();
	void initiate(int from = 0);
public:
	GameObjectHandler(int capacity = 10);
	virtual~GameObjectHandler();

	GameObject* getGameObject(int index) const;
};