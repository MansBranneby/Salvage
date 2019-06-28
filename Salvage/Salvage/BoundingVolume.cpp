#include "BoundingVolume.h"

void BoundingVolume::createBuffers(ID3D11Device * device, std::vector<BoundingVolumeVertex> vertices, std::vector<int> indices)
{
	//CREATE VERTEX BUFFER
	D3D11_BUFFER_DESC vdesc;
	vdesc.Usage = D3D11_USAGE_IMMUTABLE;
	vdesc.ByteWidth = sizeof(BoundingVolumeVertex) * (UINT)vertices.size();
	vdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vdesc.CPUAccessFlags = 0;
	vdesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertices[0];

	HRESULT hr = device->CreateBuffer(&vdesc, &initData, &_vertexBuffer);
	if (FAILED(hr))
		MessageBox(NULL, L"Error BoundingVolume VertexBuffer", L"Error", MB_OK | MB_ICONERROR);

	//CREATE INDEX BUFFER
	D3D11_BUFFER_DESC idesc;
	idesc.Usage = D3D11_USAGE_IMMUTABLE;
	idesc.ByteWidth = sizeof(int) * (UINT)indices.size();
	idesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idesc.CPUAccessFlags = 0;
	idesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	hr = device->CreateBuffer(&idesc, &initData, &_indexBuffer);
	if (FAILED(hr))
		MessageBox(NULL, L"Error BoundingVolume Index Buffer", L"Error", MB_OK | MB_ICONERROR);
}

BoundingVolume::BoundingVolume()
{
}

BoundingVolume::~BoundingVolume()
{

	if(_vertexBuffer)
		_vertexBuffer->Release();
	if(_indexBuffer)
		_indexBuffer->Release();
}

std::vector<BoundingVolumeVertex>* BoundingVolume::getVertices()
{
	return &_vertices;
}

std::vector<int>* BoundingVolume::getIndices()
{
	return &_indices;
}

void BoundingVolume::setCenter(DirectX::XMVECTOR center)
{
	_center = center;
}

void BoundingVolume::draw(ID3D11DeviceContext * deviceContext, ID3D11Buffer * transformationBuffer)
{
	UINT32 vertexSize = sizeof(BoundingVolumeVertex);
	UINT32 offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &transformationBuffer);

	deviceContext->DrawIndexed((UINT)_indices.size(), 0, 0);
}
