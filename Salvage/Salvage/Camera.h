#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

// DirectXTK
#include "Keyboard.h"
#include "Mouse.h"
#include "GamePad.h"

#include "ViewFrustum.h"

using namespace DirectX;

enum cameraMode {DEBUG, GAME};

class Camera
{
private:
	// Cameramode decides which mode the camera should be in
	cameraMode _cameraMode;

	XMVECTOR _position;
	
	XMVECTOR _lookAt;
	XMVECTOR _up;
	XMVECTOR _right;
	XMVECTOR _forward;

	XMFLOAT3 _velocity;
	XMMATRIX _rotation;
	float _distancePerSec;

	float _yaw;
	float _pitch;
	float _theta;
	float _phi;
	float _camDistance;
	float _rotationGain; // radian/sec 

	//XMMATRIX _world;
	XMMATRIX _view;
	XMMATRIX _projection;
	XMMATRIX _viewProjection;

	ViewFrustum _viewFrustum;

	void updateWVP();
	void updateDebugCam(DirectX::Keyboard::State kb, DirectX::Mouse::State ms, float deltaSeconds);
	void updateGameCam(DirectX::Keyboard::State kb, DirectX::Mouse::State ms, float deltaSeconds);

	//Smooth cam
	float t;
	float _smoothSpeed;
	float _lookAtSpeed;
	XMVECTOR _offSet;
	XMVECTOR lerp(XMVECTOR startPos, XMVECTOR endPos, float deltaSeconds);

public:
	Camera();
	Camera(ID3D11Device* device, float width, float height);
	~Camera();

	void updateRotation(float deltaAngle);

	void setVelocity(XMFLOAT3 velocity);

	XMFLOAT3 getVelocity() const;
	ViewFrustum getViewFrustum();

	// DEBUG and GAME mode
	void setMode(cameraMode cameraMode);
	void setSmoothSpeed(float smoothSpeed);
	void setLookAtSpeed(float lookAtSpeed);

	DirectX::XMVECTOR getPosition();
	DirectX::XMMATRIX getViewProjection();
	void update(DirectX::Keyboard::State kb, DirectX::Mouse::State ms, float deltaSeconds);
	void followObject(DirectX::XMVECTOR objPosition, float deltaSeconds);

};