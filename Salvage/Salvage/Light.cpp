#include "Light.h"

//void Light::createConstantBuffer(ID3D11Device * device, ID3D11DeviceContext* deviceContext)
//{
//	//CONSTANT BUFFER
//	D3D11_BUFFER_DESC cbDesc;
//	cbDesc.ByteWidth = sizeof(Light);
//	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
//	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	cbDesc.MiscFlags = 0;
//	cbDesc.StructureByteStride = 0;
//
//	// Fill in the subresource data.
//	D3D11_SUBRESOURCE_DATA InitData;
//	InitData.pSysMem = this;
//	InitData.SysMemPitch = 0;
//	InitData.SysMemSlicePitch = 0;
//
//	// create a Constant Buffer
//	HRESULT result = device->CreateBuffer(&cbDesc, &InitData, &_lightConstantBuffer);
//	if (FAILED(result))
//		MessageBox(NULL, L"Error lightConstantBuffer in Light.cpp", L"Error", MB_OK | MB_ICONERROR);
//
//	deviceContext->PSSetConstantBuffers(0, 1, &_lightConstantBuffer);
//}

Light::Light(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	_position = { 0.0f, 0.0f, 0.0f, 1.0f};
	_colour = { 1.0f, 1.0f, 1.0f, 0.0f };

	//createConstantBuffer(device, deviceContext);
}

Light::Light(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 colour)
{
	_position = position;
	_colour = colour;

	//createConstantBuffer(device, deviceContext);
}
