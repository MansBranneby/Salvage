#include "Camera.h"

void Camera::updateWVP()
{
}

Camera::Camera()
{
}

Camera::Camera(ID3D11Device* device, float width, float height)
{
	// Default mode for camera
	_cameraMode = DEBUG;

	// Setup starting camera properties
	_position = XMFLOAT4(0.0f, 0.0f, -2.0f, 1.0f);
	
	_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	_lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	_lookAt = XMVector3Normalize(_lookAt);

	_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// Setup space matricies
	_world = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	_view = XMMatrixLookAtLH(XMLoadFloat4(&_position), _lookAt, _up);
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

void Camera::updatePositon()
{
	//Update camera position
	if (_cameraMode == DEBUG)
	{
		XMVECTOR pos = XMLoadFloat4(&_position);

		pos += _velocity.x * _right;
		pos += _velocity.y * _up;
		pos += _velocity.z * _forward;
		_lookAt += pos;
		_lookAt = XMVector3Normalize(_lookAt);
		_position = XMFLOAT4(XMVectorGetX(pos), XMVectorGetY(pos), XMVectorGetZ(pos), 1.0f);
		
	
		//Update camera matrices	
		_view = XMMatrixLookAtLH(XMLoadFloat4(&_position), _lookAt, _up);
		_view = XMMatrixTranspose(_view);
		_WVP = XMMatrixMultiply(_projection, XMMatrixMultiply(_view, _world));
	}

}
