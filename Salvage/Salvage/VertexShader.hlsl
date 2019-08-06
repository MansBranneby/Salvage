struct VS_IN
{
	float3 pos      : POSITION;
	float3 normal   : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct VS_OUT
{
	float4 posW    : WORLDPOSITION;
	float4 posH      : SV_POSITION;
	float3 normal	: NORMAL;
	float2 texCoord : TEXCOORD;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix VP;
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

	output.posW = mul(float4(input.pos, 1.0f), world);
	output.posH = mul(output.posW, VP);
	output.texCoord = input.texCoord;
	output.normal = mul(input.normal, (float3x3)world);

	return output;
}