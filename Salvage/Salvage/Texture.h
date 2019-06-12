#pragma once
#include <string>
#include <d3d11.h>

class Texture
{
private:

	void createSRV();

public:
	std::string _type;
	std::string _path;
	ID3D11ShaderResourceView* _texture;
	
	Texture();
	~Texture();
};