#include "Texture.h"

void Texture::createSRV(ID3D11Device* device)
{
	// GLENN
	std::wstring filenameWS = std::wstring(_path.begin(), _path.end());
	HRESULT hr = CoInitialize(NULL);
	hr = DirectX::CreateWICTextureFromFile(device, filenameWS.c_str(), NULL, &_texture);
	if (FAILED(hr))
		MessageBox(NULL, L"Error in function createSRV in Texture.cpp", L"Error!", MB_ICONERROR | MB_OK);

	hr = NULL;


}

Texture::Texture()
{
}

Texture::Texture(ID3D11Device * device, std::string path)
{
	_path = path;
	createSRV(device);
}

Texture::~Texture()
{

}
