struct VS_IN
{
	float3 pos      : POSITION;
	float3 normal   : NORMAL;
	float2 texCoord : TEXCOORD;
	int boneIDs[4]      : ID;
	float boneWeights[4] : WEIGHT;
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
	matrix bones[4];
	bool hasAnimation;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	if (hasAnimation == true)
	{
		matrix boneTransform = bones[input.boneIDs[0]] * input.boneWeights[0];
		boneTransform += bones[input.boneIDs[1]] * input.boneWeights[1];
		boneTransform += bones[input.boneIDs[2]] * input.boneWeights[2];
		boneTransform += bones[input.boneIDs[3]] * input.boneWeights[3];

		float4 posL = mul(float4(input.pos, 1.0f), boneTransform);
		output.posW = mul(posL, world);
		output.posH = mul(output.posW, VP);
	}
	else
	{
		output.posW = mul(float4(input.pos, 1.0f), world);
		output.posH = mul(output.posW, VP);
	}

	output.texCoord = input.texCoord;
	output.normal = mul(input.normal, (float3x3)world);

	return output;
}