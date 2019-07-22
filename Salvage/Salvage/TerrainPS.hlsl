Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);

SamplerState SampleType;

struct PS_IN
{
	float4 posWS    : WORLDPOSITION;
	float4 pos		: SV_POSITION;
	float3 normal   : NORMAL;
	float2 texCoord : TEXCOORD;
};

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float4 lightPos;
	float4 lightCol;
};

float4 PS_main(PS_IN input) : SV_Target
{
	float4 texColour0 = texture0.Sample(SampleType, input.texCoord);
	float4 texColour1 = texture1.Sample(SampleType, input.texCoord);
	float4 texColour2 = texture2.Sample(SampleType, input.texCoord);
	float4 texColour3 = texture3.Sample(SampleType, input.texCoord);

	float4 baseColor = texColour0;



	float4 ambientCol = texColour0 * 0.1f;
	float diffuseFactor = saturate(dot(float4(input.normal, 0.0f), normalize(lightPos - input.posWS)));

	float4 finalCol = texColour0 * diffuseFactor * lightCol + ambientCol;

	return finalCol;
};