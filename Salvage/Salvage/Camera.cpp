#include "Camera.h"

void Camera::updateWVP()
{
}

Camera::Camera()
{
}

Camera::Camera(ID3D11Device* device, float width, float height)
{
	// Setup starting camera properties
	_position = XMFLOAT4(0.0f, 0.0f, -5.0f, 1.0f);
	
	_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	_lookAt = _forward;
	_lookAt = XMVector3Normalize(_lookAt);

	_velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// Setup space matricies
	_world = XMMatrixTranslation(_position.x, _position.y, _position.z);
	_view = XMMatrixLookAtLH(XMLoadFloat4(&_position), _lookAt, _up);
	_projection = XMMatrixPerspectiveFovLH(0.45f * DirectX::XM_PI, width / height, 0.1f, 200.0f);

	_view = XMMatrixTranspose(_view);
	_projection = XMMatrixTranspose(_projection);

	_WVP = XMMatrixMultiply(_projection, XMMatrixMultiply(_view, _world));

	//Constant Buffer
	_constantBuffer.createConstantBuffer(device, &_WVP, sizeof(_WVP));
}

Camera::~Camera()
{
}

void Camera::updateRotation(float deltaAngle)
{
}

XMMATRIX* Camera::getWVP()
{
	return &_WVP;
}

ID3D11Buffer** Camera::getConstantBuffer()
{
	return _constantBuffer.getConstantBuffer();
}

void Camera::updatePositon(XMVECTOR deltaPos)
{
}
