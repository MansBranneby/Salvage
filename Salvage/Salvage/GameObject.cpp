#include "GameObject.h"

void GameObject::updateTransformations(XMFLOAT3 position)
{
	_model.updateTransformation(position);
}

GameObject::GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR position)
{
	_position = position;
	_objectType = objType;
	createModel(device, deviceContext);

	//Initial orientation of object
	_lookAt = DirectX::XMVectorSet(0.0f, 0.f, 1.0f, 0.0f);
} 

GameObject::~GameObject()
{
}

bool GameObject::createModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	switch (_objectType)
	{
	case PLAYER:
		_model.loadModel(device, deviceContext, ".\\Resources\\Models\\gubbe1Ani.dae");
		_model.updateTransformation(XMFLOAT3(XMVectorGetX(_position), XMVectorGetY(_position), XMVectorGetZ(_position)));
		break;
	case STATICOBJECT:
		_model.loadModel(device, deviceContext, ".\\Resources\\Models\\gubbe1.dae");
		_model.updateTransformation(XMFLOAT3(XMVectorGetX(_position), XMVectorGetY(_position), XMVectorGetZ(_position)));
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

XMVECTOR GameObject::getPosition()
{
	return _position;
}
