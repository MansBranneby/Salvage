#pragma once
#include <string>
#include <d3d11.h>

class Texture
{
private:
	std::string _path;
	ID3D10ShaderResourceView* _texture;

	void createSRV();

public:
	Texture();
	~Texture();
};