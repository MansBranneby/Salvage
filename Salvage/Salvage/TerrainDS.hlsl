Texture2D gHeightMap : register(t5);

SamplerState SampleType  : register(s0);;

struct DS_IN
{
	float3 posW    : POSITION;
	float2 tex     : TEXCOORD0;
	float2 boundsY : TEXCOORD1;
};

struct DS_OUT
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float2 tex : TEXCOORD0;
	float2 tiledTex : TEXCOORD1;
};

struct PatchTess
{
	float EdgeTess[4] : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};


cbuffer DS_CONSTANT_BUFFER : register(b0)
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
	// How much to tile the texture layers.
	float2 gTexScale;
};

[domain("quad")]
DS_OUT DS_main(PatchTess patchTess, float2 uv : SV_DomainLocation, const OutputPatch<DS_IN, 4> quad)
{
	DS_OUT dout;
	// Bilinear interpolation.
	dout.posW = lerp(
		lerp(quad[0].posW, quad[1].posW, uv.x),
		lerp(quad[2].posW, quad[3].posW, uv.x),
		uv.y);
	dout.tex = lerp(
		lerp(quad[0].tex, quad[1].tex, uv.x),
		lerp(quad[2].tex, quad[3].tex, uv.x),
		uv.y);
	// Tile layer textures over terrain.
	dout.tiledTex = dout.tex * gTexScale;
	// Displacement mapping
	dout.posW.y = gHeightMap.SampleLevel(SampleType, dout.tex, 0).r;
	// NOTE: We tried computing the normal in the domain shader
	// using finite difference, but the vertices move continuously
	// with fractional_even which creates noticable light shimmering
	// artifacts as the normal changes. Therefore, we moved the
	// calculation to the pixel shader.
	// Project to homogeneous clip space.
	dout.posH = mul(float4(dout.posW, 1.0f), WVP);
	return dout;
}