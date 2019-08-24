#pragma once
#include "Model.h"
#include "GraphicResources.h"
using namespace DirectX;

enum ObjectType //Troligtvis ett tillf�lligt s�tt att hantera detta.
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
	ObjectType _objectType;
	std::string _modelDirectory = ".\\Resources\\Models\\";
	XMVECTOR _position;
	

protected:
	XMVECTOR _lookAt;
	void updateTransformations(XMFLOAT3 position);

public:
	GameObject(ObjectType objectType, DirectX::XMVECTOR startingPosition);
	GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR position, std::string modelFile);
	virtual ~GameObject() = 0;

	Model* getModel();
	bool createModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string modelFile); //Return true if everything works out
	void draw(GraphicResources* graphicResources);
	void draw(GraphicResources* graphicResources, float timeInSec);
	void drawBoundingVolume(GraphicResources* graphicResources);

	void setPosition(DirectX::XMVECTOR position);
	void setHeight(float height);

	DirectX::XMVECTOR getPositionVector();
	DirectX::XMFLOAT3 getPositionFloat3();

	virtual BoundingVolume* getBoundingVolume();

};