cbuffer ConstantBuffer : register(b1)
{
	float4 resolution;
};

struct VertexInput
{
	float2 position : TEXCOORD0;
	float2 texCoord : TEXCOORD1;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD2;
};

VertexOutput mainVS(VertexInput input)
{
	VertexOutput output;

	output.texCoord = input.texCoord;
	output.position = float4(input.position, 0.0, 1.0);

	return output;
}

Texture2D mainTexture : register(t0);
Texture3D texture0 : register(t1);

SamplerState mainSampler;

float map(float3 p)
{
	return length(p) - 0.5;
}

float4 mainPS(VertexOutput input) : SV_TARGET
{
	float4 texColor = mainTexture.Sample(mainSampler, input.texCoord);
	float4 tex2Color = texture0.Sample(mainSampler, float3(input.texCoord, abs(sin(resolution.z))));
	return texColor + tex2Color;// float4(input.pixelPos, 0.0, 1.0);
}
