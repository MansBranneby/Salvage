Texture2D diffTexture : register(t0);
SamplerState SampleType;

struct PS_IN
{
	float4 posWS    : WORLDPOSITION;
	float4 pos      : SV_POSITION;
	float3 normal	: NORMAL;
	float2 texCoord : TEXCOORD;
};

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float4 lightPos;
	float4 lightCol;
};

float4 PS_main(PS_IN input) : SV_Target
{
	float4 texColour = diffTexture.Sample(SampleType, input.texCoord);
	float4 ambientCol = texColour * 0.1f;
	float diffuseFactor = saturate(dot(float4(input.normal, 0.0f), normalize(lightPos - input.posWS)));
	
	float4 finalCol = texColour * diffuseFactor * lightCol + ambientCol;

	return finalCol;
};