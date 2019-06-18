#include "GameObjectHandler.h"

GameObjectHandler::GameObjectHandler(int capacity)
{
	_nrOfObjects = 0;
	_capacity = capacity;
	_gameObjects = new GameObject*[_capacity];
	initiate();
}

void GameObjectHandler::freeMemory()
{
	for (size_t i = 0; i < _nrOfObjects; i++)
		delete _gameObjects[i];
	delete[] _gameObjects;
}

void GameObjectHandler::expand()
{
	//Expanding double pointer holding gameObjects
	_capacity += 10;
	GameObject** temp = new GameObject*[_capacity];

	for (size_t i = 0; i < _nrOfObjects; i++)
		temp[i] = _gameObjects[i];
	delete[] _gameObjects;
	_gameObjects = temp;
	initiate(_nrOfObjects);
}

void GameObjectHandler::initiate(int from)
{
	for (size_t i = from; i < _capacity; i++)
		_gameObjects[i] = nullptr;
}

GameObjectHandler::GameObjectHandler(int capacity)
{
}

GameObjectHandler::~GameObjectHandler()
{
	freeMemory();
}

GameObject * GameObjectHandler::getGameObject(int index) const
{
	return nullptr;
}
