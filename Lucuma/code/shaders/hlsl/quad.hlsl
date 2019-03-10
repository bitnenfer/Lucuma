Texture2D mainTexture : register(t0);
SamplerState mainSampler : register(s0);

struct VertexInput
{
	float3 position : TEXCOORD0;
	float2 texCoord : TEXCOORD1; 
};

struct VertexOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD2;
};

VertexOutput quadVS(VertexInput input)
{
	VertexOutput output;
	output.position = float4(input.position, 1.0);
	output.texCoord = input.texCoord;
	return output;
}

float4 quadPS(VertexOutput input) : SV_TARGET
{
	return mainTexture.Sample(mainSampler, input.texCoord);
}
