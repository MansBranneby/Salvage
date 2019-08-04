Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);

Texture2D blendMapTexture : register(t4);

SamplerState SampleType : register(s0);

struct PS_IN
{
	float4 posH     : SV_POSITION;
	float3 posW     : POSITION;
	float2 tex      : TEXCOORD0;
	float2 tiledTex : TEXCOORD1;

};

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float4 lightPos;
	float4 lightCol;
};

float4 PS_main(PS_IN input) : SV_Target
{
	//// Sample texture layers with tiled coordinates
	float4 texColour0 = texture0.Sample(SampleType, input.tiledTex);
	float4 texColour1 = texture1.Sample(SampleType, input.tiledTex);
	float4 texColour2 = texture2.Sample(SampleType, input.tiledTex);
	float4 texColour3 = texture3.Sample(SampleType, input.tiledTex);

	// Sample blend map with calculated coordinates
	float4 blendMap = blendMapTexture.Sample(SampleType, input.tex);

	//// Blend texture layers
	float4 colour = texColour0;
	colour = lerp(colour, texColour1, blendMap.r);
	colour = lerp(colour, texColour2, blendMap.g);
	colour = lerp(colour, texColour3, blendMap.b);

	// Diffuse shading
	//float4 ambientCol = colour * 0.1f;
	//float diffuseFactor = saturate(dot(float4(input.normal, 0.0f), normalize(lightPos - input.posWS)));
	//float4 finalCol = colour * diffuseFactor * lightCol + ambientCol;

	return colour;
};