cbuffer ConstantBuffer : register(b0)
{
	float4 resolution;
};

struct VertexIn
{
	float2 position : TEXCOORD0;
	float2 texCoord : TEXCOORD1;
	float4 color : TEXCOORD2;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD3;
	float4 color : TEXCOORD4;
};

VertexOut mainVS(VertexIn input)
{
	VertexOut output;

	output.position = float4(((input.position / resolution.xy) * 2.0 - 1.0) * float2(1.0, -1.0), 0.0, 1.0);
	output.texCoord = input.texCoord;
	output.color = input.color;

	return output;
}

Texture2D mainTexture;
SamplerState mainSampler;

float4 mainPS(VertexOut input) : SV_TARGET
{
	float4 texColor = mainTexture.Sample(mainSampler, input.texCoord);
	return texColor * input.color;
}
