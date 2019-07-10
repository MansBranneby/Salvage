#include "Level.h"

Level::Level(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int levelNr)
{
	_objHandler = new GameObjectHandler(device, deviceContext);
	_levelAI.createLevel(levelNr, _objHandler);
}

Level::~Level()
{
	delete _objHandler;
}

void Level::drawLevel()
{
	_objHandler->drawObjects();
	_objHandler->drawTerrain();
}

int Level::getNrOfGameObjects() const
{
	return _objHandler->getNrOfObjects();
}

GameObject * Level::getGameObject(int index)
{
	return _objHandler->getGameObject(index);
}

Terrain * Level::getTerrain(int index)
{
	return _objHandler->getTerrain()[index];
}
