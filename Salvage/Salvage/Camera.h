#pragma once

#include <DirectXMath.h>
#include "ConstantBuffer.h"

using namespace DirectX;

enum cameraMode {DEBUG, GAME};

class Camera
{
private:
	// Cameramode decides which mode the camera should be in
	cameraMode _cameraMode;

	XMFLOAT4 _position;
	
	XMVECTOR _lookAt;
	XMVECTOR _up;
	XMVECTOR _right;
	XMVECTOR _forward;

	XMFLOAT3 _velocity;
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

	void updatePositon();
	void updateRotation(float deltaAngle);

	void setVelocity(XMFLOAT3 velocity);
	XMFLOAT3 getVelocity() const;
	
	// DEBUG and GAME mode
	void setMode(cameraMode cameraMode);

	XMMATRIX* getWVP();
	ID3D11Buffer** getConstantBuffer();
};