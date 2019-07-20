#include "Level.h"

Level::Level(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int levelNr)
{
	_objHandler = new GameObjectHandler(device, deviceContext);
	_levelAI.createLevel(levelNr, _objHandler);
}

Level::~Level()
{
	delete _objHandler;
}

void Level::drawLevel(GraphicResources* graphicResources)
{
	// GLENN
	graphicResources->getDeviceContext()->VSSetShader(&graphicResources->getShaderHandler()->getObjectVS()->getVertexShader(), nullptr, 0);
	graphicResources->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	graphicResources->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	graphicResources->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	graphicResources->getDeviceContext()->PSSetShader(&graphicResources->getShaderHandler()->getObjectPS()->getPixelShader(), nullptr, 0);

	graphicResources->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphicResources->getDeviceContext()->IASetInputLayout(&graphicResources->getShaderHandler()->getObjectVS()->getVertexLayout());
	graphicResources->getDeviceContext()->PSSetSamplers(0, 1, graphicResources->getSamplerState());

	_objHandler->drawPlayer();
	_objHandler->drawObjects();
	_objHandler->drawTerrain();

}

int Level::getNrOfGameObjects() const
{
	return _objHandler->getNrOfObjects();
}

GameObject * Level::getGameObject(int index)
{
	return _objHandler->getGameObject(index);
}

Terrain * Level::getTerrain(int index)
{
	return _objHandler->getTerrain()[index];
}

Player * Level::getPlayer()
{
	return _objHandler->getPlayer();
}
