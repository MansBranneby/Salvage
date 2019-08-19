#pragma once
#include "Level.h"
#include "GraphicResources.h"

class LevelHandler
{
private:
	Level* _level = nullptr;
	int _activeLevel;
public:
	LevelHandler(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~LevelHandler();

	void drawLevel(GraphicResources* graphicResources, float timeInSec);
	int getNrOfGameObjects() const;
	GameObject* getGameObject(int index);
	Terrain* getTerrain(int index);
	Player* getPlayer();
};