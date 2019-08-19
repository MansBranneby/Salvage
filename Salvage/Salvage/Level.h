#pragma once
#include "LevelAI.h"
#include "GraphicResources.h"

class Level
{
private:
	LevelAI _levelAI;
	GameObjectHandler* _objHandler;

public:
	Level(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int levelNr);
	~Level();

	void drawLevel(GraphicResources* graphicResources, float timeInSec);
	int getNrOfGameObjects() const;
	GameObject* getGameObject(int index);
	Terrain* getTerrain(int index);
	Player* getPlayer();
};