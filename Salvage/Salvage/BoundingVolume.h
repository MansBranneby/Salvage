#pragma once
#include "Drawable.h"
#include <DirectXMath.h>
#include <vector>

enum boundingVolumes { ORIENTED_BOUNDING_BOX};

struct BoundingVolumeVertex
{
	float x, y, z;
	float r, g, b;
};

class BoundingVolume : public Drawable
{
private:
	std::vector<BoundingVolumeVertex> _vertices;
	std::vector<int> _indices;

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;

protected:
	DirectX::XMVECTOR _center;
	//Local axes
	DirectX::XMVECTOR _xAxis;
	DirectX::XMVECTOR _yAxis;
	DirectX::XMVECTOR _zAxis;

	virtual void createBuffers(ID3D11Device* device, std::vector<BoundingVolumeVertex> vertices, std::vector<int> indices);

public:
	BoundingVolume();
	virtual ~BoundingVolume();
	virtual DirectX::XMVECTOR getCenter();
	std::vector<BoundingVolumeVertex>* getVertices();
	std::vector<int>* getIndices();

	virtual void setCenter(DirectX::XMVECTOR center);

	void draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* transformationBuffer);
	
	virtual bool intersectsWithOBB(BoundingVolume* other) = 0;
};