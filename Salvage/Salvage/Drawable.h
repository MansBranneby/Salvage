#pragma once
#include <d3d11.h>

class Drawable
{
private:
	//Kanske en pekare till deviceContext här??
public:
	Drawable();
	virtual ~Drawable();

	virtual void draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* transformationBuffer) = 0;
};