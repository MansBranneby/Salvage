#include "GameObject.h"

void GameObject::updateTransformations(XMFLOAT3 position)
{
	_model.updateTransformation(position);
}

GameObject::GameObject(ObjectType objectType, DirectX::XMVECTOR startingPosition)
{
	_position = startingPosition;
	_objectType = objectType;
}

GameObject::GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, DirectX::XMVECTOR position, std::string modelFile)
{
	_position = position;
	_objectType = objType;
	createModel(device, deviceContext, modelFile);
} 

GameObject::~GameObject()
{
}

Model * GameObject::getModel()
{
	return &_model;
}

bool GameObject::createModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string modelFile)
{
	switch (_objectType)
	{
	case PLAYER:
		_model.loadModel(device, deviceContext, _modelDirectory + modelFile);
		_model.updateTransformation(XMFLOAT3(XMVectorGetX(_position), XMVectorGetY(_position), XMVectorGetZ(_position)));
		_model.getBoundingVolume()->move(getBoundingVolume()->getCenter() + _position);
		break;
	case STATICOBJECT:
		_model.loadModel(device, deviceContext, _modelDirectory + modelFile);
		_model.updateTransformation(XMFLOAT3(XMVectorGetX(_position), XMVectorGetY(_position), XMVectorGetZ(_position)));
		_model.getBoundingVolume()->move(_position);
		break;
	default:
		break;
	};
	return true;
}

void GameObject::draw(GraphicResources* graphicResources, float timeInSec)
{
	_model.draw(graphicResources, timeInSec);
}

void GameObject::draw(GraphicResources* graphicResources)
{
	_model.draw(graphicResources);
}

void GameObject::drawBoundingVolume(GraphicResources* graphicResources)
{
	_model.drawBoundingVolume(graphicResources);
}

XMVECTOR GameObject::getPositionVector()
{
	return _position;
}

DirectX::XMFLOAT3 GameObject::getPositionFloat3()
{
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, _position);

	return position;
}

void GameObject::setPosition(DirectX::XMVECTOR position)
{
	_position = position;
}

void GameObject::setHeight(float height)
{
	_position = DirectX::XMVectorSet(DirectX::XMVectorGetX(_position), height + 1.5f, DirectX::XMVectorGetZ(_position), 1.0f);
	getBoundingVolume()->setCenter(DirectX::XMVectorSet(DirectX::XMVectorGetX(_position), height + 1.5f, DirectX::XMVectorGetZ(_position), 1.0f));
}

BoundingVolume * GameObject::getBoundingVolume()
{
	return _model.getBoundingVolume();
}
