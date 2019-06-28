#pragma once
#include "Level.h"

class LevelHandler
{
private:
	Level* _level = nullptr;
	int _activeLevel;
public:
	LevelHandler(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~LevelHandler();

	void drawLevel();
	GameObject* getGameObject(int index);
};