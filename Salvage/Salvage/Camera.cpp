#include "Camera.h"

void Camera::updateWVP()
{
}

void Camera::updateDebugCam(DirectX::Keyboard::State kb, DirectX::Mouse::State ms, float deltaSeconds)
{
	_velocity = { 0.0f, 0.0f, 0.0f };
	_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_lookAt = XMVectorSet(0.0f, 0.0f, 0.0001f, 0.0f);

	if (ms.positionMode == Mouse::MODE_RELATIVE)
	{
		_yaw += XMConvertToRadians((float)ms.x * deltaSeconds * 2000);
		_pitch += XMConvertToRadians((float)ms.y * deltaSeconds * 2000);
		_pitch = min(XM_PI / 2.0f - 0.0001f, max(-XM_PI / 2.0f + 0.0001f, _pitch));
	}

	_rotation = XMMatrixRotationRollPitchYaw(_pitch, _yaw, 0.0f);
	_right = XMVector3TransformCoord(_right, _rotation);
	_up = XMVector3TransformCoord(_up, _rotation);
	_forward = XMVector3TransformCoord(_forward, _rotation);
	_lookAt = XMVector3TransformCoord(_lookAt, _rotation);

	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	if (kb.W) //Forward
		_velocity.z = _distancePerSec * deltaSeconds;
	if (kb.S) //Backwards
		_velocity.z = -_distancePerSec * deltaSeconds;
	if (kb.A)	//Left
		_velocity.x = -_distancePerSec * deltaSeconds;
	if (kb.D)	//Right
		_velocity.x = _distancePerSec * deltaSeconds;
	if (kb.Space) //Up
		_velocity.y = _distancePerSec * deltaSeconds;
	if (kb.LeftControl) //Down
		_velocity.y = -_distancePerSec * deltaSeconds;
	
	// Update position
	_position += _velocity.x * _right;
	_position += _velocity.y * _up;
	_position += _velocity.z * _forward;
	// Update where the camera is looking;
	_lookAt += _position;
}

void Camera::updateGameCam(DirectX::Keyboard::State kb, DirectX::Mouse::State ms, float deltaSeconds)
{
	_lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	if (ms.positionMode == Mouse::MODE_RELATIVE)
	{
		_phi += XMConvertToRadians((float)ms.y * deltaSeconds * 500);
		_theta += XMConvertToRadians((float)ms.x * deltaSeconds * 500);
	}

	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	if (kb.W) //Forward
		_phi += _rotationGain * deltaSeconds;
	if (kb.S) //Backwards
		_phi -= _rotationGain * deltaSeconds;
	if (kb.A) //Left
		_theta -= _rotationGain * deltaSeconds;
	if (kb.D) //Right
		_theta += _rotationGain * deltaSeconds;

	// Calculate position based on spherical coordinates
	_position = { _camDistance * cos(_theta)* sin(_phi), _camDistance * cos(_phi), _camDistance * sin(_theta)* sin(_phi), 1.0f };
}

Camera::Camera()
{
}

Camera::Camera(ID3D11Device* device, float width, float height)
{
	// Default mode for camera
	_cameraMode = DEBUG;

	// Setup starting camera properties
	_position = { 0.0f, 0.0f, -2.0f, 1.0f };
	
	// Base vectors
	_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Where camera should look
	_lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// Camera velocity
	_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// Camera speed in moving direction
	_distancePerSec = 5.0f;

	// Rotation
	_yaw = 0;
	_pitch = 0;
	_rotationGain = 5.0f;

	// Distance from camera
	_camDistance = 5.0f;

	// Setup space matricies
	_world = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	_view = XMMatrixLookAtLH(_position, _lookAt, _up);
	_projection = XMMatrixPerspectiveFovLH(0.45f * DirectX::XM_PI, width / height, 0.1f, 200.0f);

	_view = XMMatrixTranspose(_view);
	_projection = XMMatrixTranspose(_projection);

	_WVP = XMMatrixMultiply(_projection, XMMatrixMultiply(_view, _world));

	//CONSTANT BUFFER
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(_WVP);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &_WVP;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// create a Constant Buffer
	HRESULT result = device->CreateBuffer(&cbDesc, &InitData, &_constantBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"Error CreateConstantBuffer", L"Error", MB_OK | MB_ICONERROR);
}

Camera::~Camera()
{
	if(_constantBuffer)
		_constantBuffer->Release();
}

void Camera::updateRotation(float deltaAngle)
{
}

void Camera::setVelocity(XMFLOAT3 velocity)
{
	_velocity = velocity;
}

XMFLOAT3 Camera::getVelocity() const
{
	return _velocity;
}

void Camera::setMode(cameraMode cameraMode)
{
	_cameraMode = cameraMode;
}

XMMATRIX* Camera::getWVP()
{
	return &_WVP;
}

ID3D11Buffer** Camera::getConstantBuffer()
{
	return &_constantBuffer;
}

void Camera::update(DirectX::Keyboard::State kb, DirectX::Mouse::State ms, float deltaSeconds)
{
	//Update camera position
	// DEBUG IS FIRST PERSON CAMERA
	// GAME IS ORBIT THIRD PERSON CAMERA
	if (_cameraMode == DEBUG)
	{
		// FIRST PERSON CAM
		updateDebugCam(kb, ms, deltaSeconds);
	}
	else if(_cameraMode == GAME)
	{
		// ORBIT CAM
		updateGameCam(kb, ms, deltaSeconds);
	}

	//Update camera matrices	
	_view = XMMatrixLookAtLH(_position, _lookAt, _up);
	_view = XMMatrixTranspose(_view);
	_WVP = XMMatrixMultiply(_projection, XMMatrixMultiply(_view, _world));
}