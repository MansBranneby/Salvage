#include "OBB.h"

OBB::OBB(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates)
{
	using namespace DirectX;

	//Local axes
	_xAxis = { 1.0f, 0.0f, 0.0f, 0.0f };
	_yAxis = { 0.0f, 1.0f, 0.0f, 0.0f };
	_zAxis = { 0.0f, 0.0f, 1.0f, 0.0f };

	//Half width, height and length
	_halfXYZ =
	{
		(maxCoordinates.x - minCoordinates.x) / 4,
		(maxCoordinates.y - minCoordinates.y) / 4,
		(maxCoordinates.z - minCoordinates.z) / 4,
	};

	//Center
	_center = 
	{ 
		(maxCoordinates.x + minCoordinates.x) / 2,
		(maxCoordinates.y + minCoordinates.y) / 2,
		(maxCoordinates.z + minCoordinates.z) / 2,
	};
	

	//Calculate vertices
	XMFLOAT3 colour{ 1.0f, 1.0f, 1.0f };
	//Near vertices
	getVertices()->push_back({ maxCoordinates.x, maxCoordinates.y, minCoordinates.z, colour.x, colour.y, colour.z }); //0. Right up near 
	getVertices()->push_back({ maxCoordinates.x, minCoordinates.y, minCoordinates.z, colour.x, colour.y, colour.z }); //1. Right down near
	getVertices()->push_back({ minCoordinates.x, maxCoordinates.y, minCoordinates.z, colour.x, colour.y, colour.z }); //2. Left up near
	getVertices()->push_back({ minCoordinates.x, minCoordinates.y, minCoordinates.z, colour.x, colour.y, colour.z }); //3. left down near

	//Far vertices
	getVertices()->push_back({ maxCoordinates.x, maxCoordinates.y, maxCoordinates.z, colour.x, colour.y, colour.z }); //4. Right up far
	getVertices()->push_back({ maxCoordinates.x, minCoordinates.y, maxCoordinates.z, colour.x, colour.y, colour.z }); //5. Right down far
	getVertices()->push_back({ minCoordinates.x, maxCoordinates.y, maxCoordinates.z, colour.x, colour.y, colour.z }); //6. Left up far
	getVertices()->push_back({ minCoordinates.x, minCoordinates.y, maxCoordinates.z, colour.x, colour.y, colour.z }); //7. left down far

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

bool OBB::intersectsWithOBB(BoundingVolume * other)
{
	using namespace DirectX;
	// dynamic_cast is useful when you don't know what the dynamic type of the object is.
	//It returns a null pointer if the object referred to doesn't contain the type casted to as a base class 
	if(OBB* otherOBB = dynamic_cast<OBB*> (other))
	{
		XMVECTOR vectorToOBB = otherOBB->_center - _center;    //Vector from center to center

		//Seperating axis (SA) = _xAxis
		float totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, _xAxis))));
		float projLength = _halfXYZ.x +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_zAxis)))));
		if ((totalProjLength/2) > projLength)
			return false;
		//2. SA = _yAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, _yAxis))));
		projLength = _halfXYZ.y +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_zAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//3. SA = _zAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, _zAxis))));
		projLength = _halfXYZ.z +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_zAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//4. SA = xAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, otherOBB->_xAxis))));
		projLength = otherOBB->_halfXYZ.x +
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_xAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//5. SA = yAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, otherOBB->_yAxis))));
		projLength = otherOBB->_halfXYZ.y +
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_yAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//6. SA = zAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, otherOBB->_zAxis))));
		projLength = otherOBB->_halfXYZ.z +
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_zAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;

		//.7 SA = _xAxis x xAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_xAxis, otherOBB->_xAxis)))));
		projLength =
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_yAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//8. SA = _xAxis x yAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_xAxis, otherOBB->_yAxis)))));
		projLength =
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_xAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//9. SA = _xAxis x zAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_xAxis, otherOBB->_zAxis)))));
		projLength =
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_xAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//10. SA = _yAxis x xAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_yAxis, otherOBB->_xAxis)))));
		projLength =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_yAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//11. SA = _yAxis x yAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_yAxis, otherOBB->_yAxis)))));
		projLength =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_xAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//12. SA = _yAxis x zAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_yAxis, otherOBB->_zAxis)))));
		projLength =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_xAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//13. SA = _zAxis x xAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_zAxis, otherOBB->_xAxis)))));
		projLength =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_yAxis)))));
		if((totalProjLength / 2) > projLength)
			return false;
		//14. SA = _zAxis x yAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_zAxis, otherOBB->_yAxis)))));
		projLength =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_xAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
		//15. SA = _zAxis x zAxis
		totalProjLength = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_zAxis, otherOBB->_zAxis)))));
		projLength =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_xAxis)))));
		if ((totalProjLength / 2) > projLength)
			return false;
	}

	return true;
}