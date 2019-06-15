#pragma once
#include "Model.h"

class GameObject
{
private:
	Model _model;
	DirectX::XMVECTOR _position;

public:
	GameObject();
	~GameObject();

	bool createModel(); //Return true if everything works out
	virtual void updateLogic() = 0;
};