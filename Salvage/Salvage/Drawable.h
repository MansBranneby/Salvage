#pragma once
#include <d3d11.h>

class Drawable
{
private:
	//Kanske en pekare till deviceContext här??
public:
	Drawable();
	~Drawable();

	virtual void draw(ID3D11DeviceContext* deviceContext) = 0;
};