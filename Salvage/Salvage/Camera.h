#pragma once

#include <DirectXMath.h>
#include "ConstantBuffer.h"

using namespace DirectX;

class Camera
{
private:
	XMFLOAT4 _position;
	
	XMVECTOR _lookAt;
	XMVECTOR _up;
	XMVECTOR _right;
	XMVECTOR _forward;

	XMVECTOR _velocity;
	XMMATRIX _rotation;
	float _angle;

	XMMATRIX _world;
	XMMATRIX _view;
	XMMATRIX _projection;

	XMMATRIX _WVP;

	ID3D11Buffer* _constantBuffer;

	void updateWVP();

public:
	Camera();
	Camera(ID3D11Device* device, float width, float height);
	~Camera();

	void updatePositon(XMVECTOR deltaPos);
	void updateRotation(float deltaAngle);

	//XMMATRIX* getWVP();
	ID3D11Buffer** getConstantBuffer();
};