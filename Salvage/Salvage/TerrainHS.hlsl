struct HS_IN
{
	float3 posW    : POSITION;
	float2 tex     : TEXCOORD0;
	float2 boundsY : TEXCOORD1;
};

struct HS_OUT
{
	float3 posW    : POSITION;
	float2 tex     : TEXCOORD0;
};

cbuffer HS_CONSTANT_BUFFER : register(b0)
{
	matrix VP;
	float4 camPos;
	float4 frustumPlanes[6];
	//// When distance is minimum, the tessellation is maximum.
	//// When distance is maximum, the tessellation is minimum.
	float gMinDist;
	float gMaxDist;
	//// Exponents for power of 2 tessellation. The tessellation
	//// range is [2^(gMinTess), 2^(gMaxTess)]. Since the maximum
	//// tessellation is 64, this means gMaxTess can be at most 6
	//// since 2^6 = 64, and gMinTess must be at least 0 since 2^0 = 1.
	float gMinTess;
	float gMaxTess;
	float2 texScale;
	float2 padding;
};

// Returns true if the box is completely behind (in negative
// half space) of plane.
bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
	float3 n = abs(plane.xyz); // (|n.x|, |n.y|, |n.z|)
	// This is always positive.
	float r = dot(extents, n);
	// signed distance from center point to plane.
	float s = dot(float4(center, 1.0f), plane);
	// If the center point of the box is a distance of e or more behind the
	// plane (in which case s is negative since it is behind the plane),
	// then the box is completely in the negative half space of the plane.
	return (s + r) < 0.0f;
}
// Returns true if the box is completely outside the frustum.
bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6])
{
	for (int i = 0; i < 6; ++i)
	{
		// If the box is completely behind any of the frustum planes
		// then it is outside the frustum.
		if (AabbBehindPlaneTest(center, extents, frustumPlanes[i]))
			return true;

	}
	return false;
}


float CalcTessFactor(float3 p)
{
	float d = distance(p, camPos.xyz);
	float s = saturate((d - gMinDist) / (gMaxDist - gMinDist));
	return pow(2, (lerp(gMaxTess, gMinTess, s)));
}

struct PatchTess
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<HS_IN, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess patchTess;

	// Frustum culling
	float minY = patch[0].boundsY.x;
	float maxY = patch[0].boundsY.y;
	float3 vMin = float3(patch[2].posW.x, minY, patch[2].posW.z);
	float3 vMax = float3(patch[1].posW.x, maxY, patch[1].posW.z);
	float3 boxCenter = 0.5f*(vMin + vMax);
	float3 boxExtents = 0.5f*(vMax - vMin);
	if (AabbOutsideFrustumTest(boxCenter, boxExtents, frustumPlanes))
	{
		patchTess.EdgeTess[0] = 0.0f;
		patchTess.EdgeTess[1] = 0.0f;
		patchTess.EdgeTess[2] = 0.0f;
		patchTess.EdgeTess[3] = 0.0f;
		patchTess.InsideTess[0] = 0.0f;
		patchTess.InsideTess[1] = 0.0f;
		return patchTess;
	}
	else
	{
		// It is important to do the tess factor calculation
		// based on the edge properties so that edges shared
		// by more than one patch will have the same
		// tessellation factor. Otherwise, gaps can appear.
		// Compute midpoint on edges, and patch center
		float3 e0 = 0.5f*(patch[0].posW + patch[2].posW);
		float3 e1 = 0.5f*(patch[0].posW + patch[1].posW);
		float3 e2 = 0.5f*(patch[1].posW + patch[3].posW);
		float3 e3 = 0.5f*(patch[2].posW + patch[3].posW);
		float3 c = 0.25f*(patch[0].posW + patch[1].posW + patch[2].posW + patch[3].posW);

		patchTess.EdgeTess[0] = CalcTessFactor(e0);
		patchTess.EdgeTess[1] = CalcTessFactor(e1);
		patchTess.EdgeTess[2] = CalcTessFactor(e2);
		patchTess.EdgeTess[3] = CalcTessFactor(e3);
		patchTess.InsideTess[0] = CalcTessFactor(c);
		patchTess.InsideTess[1] = patchTess.InsideTess[0];

		return patchTess;
	}
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HS_OUT HS_main(InputPatch<HS_IN, 4> p, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HS_OUT output;


	output.posW = p[i].posW;
	output.tex = p[i].tex;

	return output;
}