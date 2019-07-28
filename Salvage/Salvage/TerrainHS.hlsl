struct VS_OUT
{
	float4 posWS     : WORLDPOSITION;
	float4 pos       : SV_POSITION;
	float3 normal	 : NORMAL;
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
};

struct PatchTess
{
	float EdgeTess[4] : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
	// Additional info you want associated per patch.
};
PatchTess ConstantHS(InputPatch<VS_OUT, 4> patch,
	uint patchID : SV_PrimitiveID)
{
	PatchTess pt;
	// Uniformly tessellate the patch 3 times.
	pt.EdgeTess[0] = 3; // Left edge
	pt.EdgeTess[1] = 3; // Top edge
	pt.EdgeTess[2] = 3; // Right edge
	pt.EdgeTess[3] = 3; // Bottom edge
	pt.InsideTess[0] = 3; // u-axis (columns)
	pt.InsideTess[1] = 3; // v-axis (rows)
	return pt;
}

struct HullOut
{
	float3 PosL : POSITION;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS_main(InputPatch<VS_OUT, 4> p, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HullOut hout;
	hout.PosL = p[i].pos;
	return hout;
}
