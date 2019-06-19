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

void LevelHandler::drawLevel()
{
	_level->drawLevel();
}
