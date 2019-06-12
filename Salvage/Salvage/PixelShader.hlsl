Texture2D diffTexture : register(t0);
SamplerState SampleType;

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 PS_main(PS_IN input) : SV_Target
{
	float4 texColour = diffTexture.Sample(SampleType, input.texCoord);
	return texColour;
};