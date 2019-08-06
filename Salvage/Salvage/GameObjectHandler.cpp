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


	for (size_t i = 0; i < _terrain.size(); i++)
		delete _terrain[i];

	if (_player)
		delete _player;
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
		_player = new Player(_device, _deviceContext, position, modelFile);
		break;
	case ENEMY:
		//_gameObjects[_nrOfObjects++] = new Enemy(_device, _deviceContext, position);
		break;
	case STATICOBJECT:
		_gameObjects[_nrOfObjects++] = new StaticObject(_device, _deviceContext, position, modelFile);
		break;
	case TERRAIN:
		_terrain.push_back(new Terrain(_device, _deviceContext, position, modelFile));
		break;
	default:
		break;
	}
}

int GameObjectHandler::getNrOfObjects() const
{
	return _nrOfObjects;
}

std::vector<Terrain*> GameObjectHandler::getTerrain()
{
	return _terrain;
}

Player * GameObjectHandler::getPlayer()
{
	return _player;
}

void GameObjectHandler::drawPlayer(GraphicResources* graphicResources)
{
	_player->draw(graphicResources);
}

void GameObjectHandler::drawTerrain(GraphicResources* graphicResources)
{
	for (size_t i = 0; i < _terrain.size(); i++)
		_terrain[i]->draw(graphicResources);
}

void GameObjectHandler::drawObjects(GraphicResources* graphicResources)
{
	for (size_t i = 0; i < _nrOfObjects; i++)
		_gameObjects[i]->draw(graphicResources);
}

void GameObjectHandler::drawPlayerBV(GraphicResources* graphicResources)
{
	_player->drawBoundingVolume(graphicResources);
}

GameObject * GameObjectHandler::getGameObject(int index) const
{
	return _gameObjects[index];
}
