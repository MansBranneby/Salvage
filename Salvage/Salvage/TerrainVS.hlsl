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
	matrix VP;
	float4 camPos;
	float4 frustumPlanes[6];
	float gMinDist;
	float gMaxDist;
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
