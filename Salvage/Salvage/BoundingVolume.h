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


public:
	BoundingVolume();
	BoundingVolume(ID3D11Device* device);
	virtual ~BoundingVolume();
	std::vector<BoundingVolumeVertex>* getVertices();
	std::vector<int>* getIndices();
	
	void createBuffers(ID3D11Device* device, std::vector<BoundingVolumeVertex> vertices, std::vector<int> indices);

	void draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* transformationBuffer);
};