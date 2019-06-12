struct VS_IN
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix WVP;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	output.pos = mul(float4(input.pos, 1), WVP);
	//output.pos = float4(input.pos, 1);
	output.texCoord = input.texCoord;

	return output;
}