#pragma once

class Vertex
{
private:
public:
	float x, y, z;
	float u, v;
	float weights[4];
	float boneIndices[4];

	Vertex();
	~Vertex();
};