#include "Mesh.h"

void Mesh::createBuffers(ID3D11Device * device, std::vector<Vertex> vertices, std::vector<int> indices)
{
	//CREATE VERTEX BUFFER
	D3D11_BUFFER_DESC vdesc;
	vdesc.Usage = D3D11_USAGE_IMMUTABLE;
	vdesc.ByteWidth = sizeof(Vertex) * vertices.size();
	vdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vdesc.CPUAccessFlags = 0;
	vdesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertices[0];

	HRESULT hr = device->CreateBuffer(&vdesc, &initData, &_vertexBuffer);
	if (FAILED(hr))
		MessageBox(NULL, L"Error Mesh VertexBuffer", L"Error", MB_OK | MB_ICONERROR);

	//CREATE INDEX BUFFER
	D3D11_BUFFER_DESC idesc;
	idesc.Usage = D3D11_USAGE_IMMUTABLE;
	idesc.ByteWidth = sizeof(int) * indices.size();
	idesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idesc.CPUAccessFlags = 0;
	idesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	hr = device->CreateBuffer(&idesc, &initData, &_indexBuffer);
	if (FAILED(hr))
		MessageBox(NULL, L"Error Mesh Index Buffer", L"Error", MB_OK | MB_ICONERROR);
}

Mesh::Mesh()
{
}

Mesh::Mesh(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<int> indices, std::vector<Texture> textures)
{
	_vertices = vertices;
	_indices = indices;
	_textures = textures;

	createBuffers(device, _vertices, _indices); //Måste nog lägga till _textures här
}

Mesh::~Mesh()
{
	//RUNTIME ERROR

	//if(_vertexBuffer != nullptr)
	//	_vertexBuffer->Release();
	//if(_indexBuffer != nullptr)
	//	_indexBuffer->Release();
}

void Mesh::draw(ID3D11DeviceContext * deviceContext)
{
	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->Draw(_vertices.size(), 0);
}
