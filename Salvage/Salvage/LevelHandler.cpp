#include "LevelHandler.h"

LevelHandler::LevelHandler(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	_activeLevel = 1;
	_level = new Level(device, deviceContext, _activeLevel);
}

LevelHandler::~LevelHandler()
{
	delete _level;
}

void LevelHandler::drawLevel(GraphicResources* graphicResources, float timeInSec)
{
	_level->drawLevel(graphicResources, timeInSec);
}

int LevelHandler::getNrOfGameObjects() const
{
	return _level->getNrOfGameObjects();
}

GameObject * LevelHandler::getGameObject(int index)
{
	return _level->getGameObject(index);
}

Terrain * LevelHandler::getTerrain(int index)
{
	return _level->getTerrain(index);
}

Player * LevelHandler::getPlayer()
{
	return _level->getPlayer();
}
