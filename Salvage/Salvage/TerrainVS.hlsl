struct VS_IN
{
	float3 pos      : POSITION;
	float3 normal   : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct VS_OUT
{
	float4 posWS    : WORLDPOSITION;
	float4 pos      : SV_POSITION;
	float3 normal	: NORMAL;
	float2 texCoord : TEXCOORD;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	matrix WVP;
};

cbuffer VS_CONSTANT_BUFFER : register(b1)
{
	matrix transformMatrix;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.posWS = output.pos = mul(float4(input.pos, 1.0f), transformMatrix);

	output.pos = mul(float4(input.pos, 1.0f), world);
	output.pos = mul(output.pos, transformMatrix);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.texCoord = input.texCoord;
	output.normal = mul(input.normal, (float3x3)transformMatrix);

	return output;
}