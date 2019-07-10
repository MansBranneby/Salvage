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

void GameObject::loadTerrainModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, size_t terrainWidth, size_t terrainHeight, std::vector<DirectX::XMFLOAT3> heightmap)
{
	_model.processHeightmap(device, deviceContext, terrainWidth, terrainHeight, heightmap);
}

void GameObject::draw(ID3D11DeviceContext * deviceContext)
{
	_model.draw(deviceContext);
}

void GameObject::drawBoundingVolume(ID3D11DeviceContext * deviceContext)
{
	_model.drawBoundingVolume(deviceContext);
}

XMVECTOR GameObject::getPosition()
{
	return _position;
}

BoundingVolume * GameObject::getBoundingVolume()
{
	return _model.getBoundingVolume();
}
