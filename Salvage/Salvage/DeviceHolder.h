#pragma once
#include <d3d11.h>

class DeviceHolder
{
public:
	static DeviceHolder& getInstance()
	{
		static DeviceHolder instance;
		return instance;
	}
private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	DeviceHolder() {}

public:
	DeviceHolder(DeviceHolder const&) = delete;
	void operator=(DeviceHolder const&) = delete;

	ID3D11Device* getDevice()
	{
		return device;
	}
	ID3D11DeviceContext* getDeviceContext()
	{
		return deviceContext;
	}
};