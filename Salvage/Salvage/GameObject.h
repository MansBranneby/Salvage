#pragma once
//#include "DeviceHolder.h" //Singleton maybe a good alternative for holding devices
#include "Model.h"
using namespace DirectX;

enum ObjectType //Troligtvis ett tillfälligt sätt att hantera detta.
{
	PLAYER,
	ENEMY,
	STATICOBJECT,
	TERRAIN
};

class GameObject
{
private:
	Model _model;
	XMMATRIX _rotMatrix;
	ObjectType _objectType;
	std::string _modelDirectory = ".\\Resources\\Models\\";
	

protected:
	XMVECTOR _position;
	XMVECTOR _lookAt;
	void updateTransformations(XMFLOAT3 position);

public:
	GameObject(ObjectType objectType, DirectX::XMVECTOR startingPosition);
	GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR position, std::string modelFile);
	virtual ~GameObject();

	Model* getModel();
	bool createModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string modelFile); //Return true if everything works out
	void draw(ID3D11DeviceContext* deviceContext);
	void drawBoundingVolume(ID3D11DeviceContext* deviceContext);
	virtual void updateLogic() = 0;

	XMVECTOR getPosition();
	virtual BoundingVolume* getBoundingVolume();
};