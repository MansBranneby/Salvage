#include "OBB.h"

OBB::OBB(DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates)
{
	using namespace DirectX;

	//Center
	XMFLOAT3 center 
	{
		(minCoordinates.x + maxCoordinates.x) / 2,
		(minCoordinates.y + maxCoordinates.y) / 2,
		(minCoordinates.z + maxCoordinates.z) / 2 
	};

	//Half width, height and length
	XMFLOAT3 halfXYZ
	{
		(abs(minCoordinates.x) + abs(maxCoordinates.x)) / 2,
		(abs(minCoordinates.y) + abs(maxCoordinates.y)) / 2,
		(abs(minCoordinates.z) + abs(maxCoordinates.z)) / 2,
	};

	//Calculate vertices
	XMFLOAT3 colour{ 1.0f, 1.0f, 1.0f };
	//Near vertices
	getVertices().push_back({ center.x + halfXYZ.x, center.y + halfXYZ.y, center.z - halfXYZ.z, colour.x, colour.y, colour.z }); //1. Right up near 
	getVertices().push_back({ center.x + halfXYZ.x, center.y - halfXYZ.y, center.z - halfXYZ.z, colour.x, colour.y, colour.z }); //2. Right down near
	getVertices().push_back({ center.x - halfXYZ.x, center.y + halfXYZ.y, center.z - halfXYZ.z, colour.x, colour.y, colour.z }); //3. Left up near
	getVertices().push_back({ center.x - halfXYZ.x, center.y - halfXYZ.y, center.z - halfXYZ.z, colour.x, colour.y, colour.z }); //4. left down near

	//Far vertices
	getVertices().push_back({ center.x + halfXYZ.x, center.y + halfXYZ.y, center.z + halfXYZ.z, colour.x, colour.y, colour.z }); //5. Right up far
	getVertices().push_back({ center.x + halfXYZ.x, center.y - halfXYZ.y, center.z + halfXYZ.z, colour.x, colour.y, colour.z }); //6. Right down far
	getVertices().push_back({ center.x - halfXYZ.x, center.y + halfXYZ.y, center.z + halfXYZ.z, colour.x, colour.y, colour.z }); //7. Left up far
	getVertices().push_back({ center.x - halfXYZ.x, center.y - halfXYZ.y, center.z + halfXYZ.z, colour.x, colour.y, colour.z }); //8. left down far

	//Indices
	//Front face
	getIndices().push_back(1); getIndices().push_back(2); //RUN -> RDN
	getIndices().push_back(2); getIndices().push_back(4); //RDN -> LDN
	getIndices().push_back(4); getIndices().push_back(3); //LDN -> LUN
	getIndices().push_back(3); getIndices().push_back(1); //LUN -> RUN
	getIndices().push_back(1); getIndices().push_back(5); //RUN -> RUF
	getIndices().push_back(5); getIndices().push_back(6); //RUF -> RDF
	getIndices().push_back(6); getIndices().push_back(2); //RDF -> RDN
	getIndices().push_back(3); getIndices().push_back(7); //LUN -> LUF
	getIndices().push_back(7); getIndices().push_back(8); //LUF -> LDF
	getIndices().push_back(8); getIndices().push_back(4); //LDF -> LDN
	getIndices().push_back(7); getIndices().push_back(5); //LUF -> RUF 
	getIndices().push_back(8); getIndices().push_back(6); //LDF -> RDF
}