Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);

Texture2D blendMapTexture : register(t4);

Texture2D gHeightMap : register(t5);

SamplerState SampleType : register(s0);
SamplerState SampleHeight : register(s1);

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

	// Calculate normals with central difference
	float2 left = input.tex + float2(-0.001953125f, 0.0f);
	float2 right = input.tex + float2(0.001953125f, 0.0f);
	float2 bottom = input.tex + float2(0.0f, 0.001953125f);
	float2 top = input.tex + float2(0.0f, -0.001953125f);

	float leftHeight = gHeightMap.SampleLevel(SampleHeight, left, 0).r;
	float rightHeight = gHeightMap.SampleLevel(SampleHeight, right, 0).r;
	float bottomHeight = gHeightMap.SampleLevel(SampleHeight, bottom, 0).r;
	float topHeight = gHeightMap.SampleLevel(SampleHeight, top, 0).r;

	float3 normalW = normalize(float3(leftHeight - rightHeight, 2.0f, bottomHeight - topHeight));
	//float3 tangent = normalize(float3(2.0f, rightHeight - leftHeight, 0.0f));
	//float3 bitangent = normalize(float3(0.0f, bottomHeight - topHeight, -2.0f));
	//float3 normalW = normalize(cross(tangent, bitangent));

	// Diffuse shading
	float4 ambientCol = colour * 0.1f;
	float diffuseFactor = saturate(dot(float4(normalW, 1.0f), normalize(lightPos - float4(input.posW, 1.0f))));
	float4 finalCol = colour * diffuseFactor * lightCol + ambientCol;

	return finalCol;
};