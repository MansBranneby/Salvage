#include "GameObject.h"

void GameObject::updateTransformations(XMFLOAT3 position)
{
	_model.updateTransformation(position);
}

GameObject::GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR position)
{
	_position = position;
	createModel(device, deviceContext, PLAYER);

	//Initial orientation of object
	_lookAt = DirectX::XMVectorSet(0.0f, 0.f, 1.0f, 0.0f);
} 

GameObject::~GameObject()
{
}

bool GameObject::createModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType)
{
	switch (objType)
	{
	case PLAYER:
		_model.loadModel(device, deviceContext, ".\\Resources\\Models\\gubbe1Ani.dae");
		break;
	case STATICOBJECT:
		break;
	default:
		break;
	};
	return true;
}

void GameObject::draw(ID3D11DeviceContext * deviceContext)
{
	_model.draw(deviceContext);
}
