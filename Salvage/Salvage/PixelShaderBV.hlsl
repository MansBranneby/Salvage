struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 col : TEXCOORD;
};

float4 PS_main(PS_IN input) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
};