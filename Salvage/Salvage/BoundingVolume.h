#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

enum BoundingVolumes { ORIENTED_BOUNDING_BOX};

struct BoundingVolumeVertex
{
	float x, y, z;
	float r, g, b;
};

class BoundingVolume
{
private:
	std::vector<BoundingVolumeVertex> _vertices;
	std::vector<int> _indices;
	DirectX::XMMATRIX _worldMatrix;

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;

	DirectX::XMVECTOR _center;

protected:
	virtual void createBuffers(ID3D11Device* device, std::vector<BoundingVolumeVertex> vertices, std::vector<int> indices);

public:
	BoundingVolume();
	BoundingVolume(DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates);
	virtual ~BoundingVolume();
	virtual DirectX::XMVECTOR getCenter();
	std::vector<BoundingVolumeVertex>* getVertices();
	std::vector<int>* getIndices();
	DirectX::XMMATRIX getWorldMatrix();

	virtual void setCenter(DirectX::XMVECTOR center);
	virtual void setWorldMatrix(DirectX::XMMATRIX worldMatrix);
	virtual void move(DirectX::XMVECTOR speed);

	virtual void draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* transformationBuffer);
	
	virtual bool intersectsWithOBB(BoundingVolume* other) = 0;
};