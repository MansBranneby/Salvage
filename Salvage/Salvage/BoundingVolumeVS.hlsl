struct VS_IN
{
	float3 pos : POSITION;
	float3 col : COLOUR;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 col : COLOUR;
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

cbuffer VS_CONSTANT_BUFFER : register(b1)
{
	matrix world;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.0f), world);
	output.pos = mul(output.pos, WVP);
	output.col = input.col;

	return output;
}