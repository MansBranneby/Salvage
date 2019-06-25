#include "OBB.h"

OBB::OBB(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates)
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
	getVertices()->push_back({ halfXYZ.x, halfXYZ.y,-halfXYZ.z, colour.x, colour.y, colour.z }); //0. Right up near 
	getVertices()->push_back({ halfXYZ.x,-halfXYZ.y,-halfXYZ.z, colour.x, colour.y, colour.z }); //1. Right down near
	getVertices()->push_back({-halfXYZ.x, halfXYZ.y,-halfXYZ.z, colour.x, colour.y, colour.z }); //2. Left up near
	getVertices()->push_back({-halfXYZ.x,-halfXYZ.y,-halfXYZ.z, colour.x, colour.y, colour.z }); //3. left down near

	//Far vertices
	getVertices()->push_back({ halfXYZ.x, halfXYZ.y, halfXYZ.z, colour.x, colour.y, colour.z }); //4. Right up far
	getVertices()->push_back({ halfXYZ.x,-halfXYZ.y, halfXYZ.z, colour.x, colour.y, colour.z }); //5. Right down far
	getVertices()->push_back({-halfXYZ.x, halfXYZ.y, halfXYZ.z, colour.x, colour.y, colour.z }); //6. Left up far
	getVertices()->push_back({-halfXYZ.x,-halfXYZ.y, halfXYZ.z, colour.x, colour.y, colour.z }); //7. left down far

	//Indices
	getIndices()->push_back(0); getIndices()->push_back(1); //RUN -> RDN
	getIndices()->push_back(1); getIndices()->push_back(3); //RDN -> LDN
	getIndices()->push_back(3); getIndices()->push_back(2); //LDN -> LUN
	getIndices()->push_back(2); getIndices()->push_back(0); //LUN -> RUN
	getIndices()->push_back(0); getIndices()->push_back(4); //RUN -> RUF
	getIndices()->push_back(4); getIndices()->push_back(5); //RUF -> RDF
	getIndices()->push_back(5); getIndices()->push_back(1); //RDF -> RDN
	getIndices()->push_back(2); getIndices()->push_back(6); //LUN -> LUF
	getIndices()->push_back(6); getIndices()->push_back(7); //LUF -> LDF
	getIndices()->push_back(7); getIndices()->push_back(3); //LDF -> LDN
	getIndices()->push_back(6); getIndices()->push_back(4); //LUF -> RUF 
	getIndices()->push_back(7); getIndices()->push_back(5); //LDF -> RDF

	createBuffers(device, *getVertices(), *getIndices());
}
