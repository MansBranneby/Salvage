#pragma once
//#include "DeviceHolder.h" //Singleton maybe a good alternative for holding devices
#include "Model.h"
using namespace DirectX;

enum ObjectType //Troligtvis ett tillfälligt sätt att hantera detta.
{
	PLAYER,
	STATICOBJECT
};

class GameObject
{
private:
	DirectX::XMMATRIX _rotMatrix;
	float yaw;
	Model _model;

protected:
	DirectX::XMVECTOR _position;
	DirectX::XMVECTOR _lookAt;
	void updateTransformations(XMFLOAT3 position);

public:

	GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR position);
	~GameObject();

	bool createModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType); //Return true if everything works out
	void draw(ID3D11DeviceContext* deviceContext);
	virtual void updateLogic() = 0;
};