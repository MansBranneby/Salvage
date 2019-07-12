#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

class Light
{
private:
	//void createConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
public:
	Light(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	Light(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 colour);
	DirectX::XMFLOAT4 _position;
	DirectX::XMFLOAT4 _colour;

	//ID3D11Buffer* _lightConstantBuffer;
};