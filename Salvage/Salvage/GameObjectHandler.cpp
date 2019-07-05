#include "GameObjectHandler.h"

GameObjectHandler::GameObjectHandler(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int capacity)
{
	_device = device;
	_deviceContext = deviceContext;

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

GameObjectHandler::~GameObjectHandler()
{
	freeMemory();
}

void GameObjectHandler::addGameObject(ObjectType objType, XMVECTOR position, std::string modelFile)
{
	if (_nrOfObjects == _capacity)
		expand();
	switch (objType)
	{
	case PLAYER:
		_gameObjects[_nrOfObjects++] = new Player(_device, _deviceContext, position, modelFile);
		break;
	case ENEMY:
		//_gameObjects[_nrOfObjects++] = new Enemy(_device, _deviceContext, position);
		break;
	case STATICOBJECT:
		_gameObjects[_nrOfObjects++] = new StaticObject(_device, _deviceContext, position, modelFile);
		break;
	default:
		break;
	}
}

int GameObjectHandler::getNrOfObjects() const
{
	return _nrOfObjects;
}

void GameObjectHandler::drawObjects()
{
	for (size_t i = 0; i < _nrOfObjects; i++)
		_gameObjects[i]->draw(_deviceContext);
}

GameObject * GameObjectHandler::getGameObject(int index) const
{
	return _gameObjects[index];
}
