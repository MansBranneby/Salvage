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
	float2 boundsY : TEXCOORD1;
};

cbuffer HS_CONSTANT_BUFFER : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	matrix WVP;
	float4 camPos;

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
};


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
	//
	// Frustum cull
	//
	//[... Omit frustum culling code]
	//
	// Do normal tessellation based on distance.
	//
	//else
	//{
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
	//}
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