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

	output.pos = mul(float4(input.pos, 1.0f), world);
	output.pos = mul(output.pos, VP);
	output.col = input.col;

	return output;
}