#pragma once
#include <string>
#include <d3d11.h>
#include <WICTextureLoader.h>


class Texture
{
private:

	void createSRV(ID3D11Device* device);

public:
	std::string _type;
	std::string _path;
	ID3D11ShaderResourceView* _texture;
	
	Texture();
	Texture(ID3D11Device* device, std::string path);
	~Texture();
};