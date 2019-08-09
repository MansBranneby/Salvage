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
	float4 frustumPlanes[6];
	float gMinDist;
	float gMaxDist;
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