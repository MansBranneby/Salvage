Texture2D gHeightMap : register(t5);

SamplerState SampleType : register(s0);

struct VS_IN
{
	float3 posL    : POSITION;
	float2 tex     : TEXCOORD0;
	float2 boundsY : TEXCOORD1;
};

struct VS_OUT
{
	float3 posW    : POSITION;
	float2 tex	   : TEXCOORD0;
	float2 boundsY : TEXCOORD1;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
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

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	// Terrain specified directly in world space.
   // output.posW = input.posL;
	output.posW = input.posL;
	// Displace the patch corners to world space. This is to make
	// the eye to patch distance calculation more accurate.
	output.posW.y = gHeightMap.SampleLevel(SampleType, input.tex, 0).r;

	// Output vertex attributes to next stage.
	output.tex = input.tex;
	output.boundsY = input.boundsY;

	return output;
}
