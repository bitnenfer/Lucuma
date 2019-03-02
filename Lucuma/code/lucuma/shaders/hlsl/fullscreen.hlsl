cbuffer ConstantBuffer : register(b0)
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

Texture2D mainTexture;
SamplerState mainSampler;

float map(float3 p)
{
	return length(p) - 0.5;
}

float4 mainPS(VertexOutput input) : SV_TARGET
{
	float4 texColor = mainTexture.Sample(mainSampler, input.texCoord);

	float3 color = float3(0.0, 0.0, 0.0);
	float2 uv = (input.texCoord -0.5) * float2(1.33, 1.0);
	float3 ro = float3(0.0, 0.0, -2.0);
	float3 rd = normalize(float3(uv, 1.0));
	float t = 0.0;


	for (int i = 0; i < 200; ++i)
	{
		float3 p = ro + rd * t;
		float d = map(p);
		if (d < 0.001)
		{
			float2 e = float2(0.0, 0.001);
			float3 n = normalize(
				d - float3(map(p - e.yxx),
				map(p - e.xyx),
				map(p - e.xxy))
			);
			color = n;
			break;
		}
		t += d;
		if (t > 10.0) break;
	}

	return texColor + float4(color, 0.0);// float4(input.pixelPos, 0.0, 1.0);
}
