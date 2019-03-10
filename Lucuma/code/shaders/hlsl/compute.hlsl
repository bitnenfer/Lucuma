
cbuffer ResolutionBuffer : register (b0)
{
	uint4 uResolution;
	float4 fResolution;
};

cbuffer DynamicBuffer : register(b1)
{
	float4 time;
};

RWTexture2D<float4> tex : register(u0);

float box(float3 p, float3 size)
{
	float3 a = abs(p) - size;
	return length(max(a, 0.0)) + min(max(a.x, max(a.y, a.z)), 0.0);
}

float sphere(float3 p, float r)
{
	return length(p) - r;
}

float2x2 rot(float r)
{
	return float2x2(cos(r), -sin(r), sin(r), cos(r));
}

float2 moda(inout float2 p, float rep)
{
	float angle = 6.28 / rep;
	float a = atan2(p.y, p.x) + angle / 2.0;
	float r = length(p);
	float c = floor(a / angle);
	a = fmod(a, angle) - angle / 2.0;
	p = float2(cos(a), sin(a))*r;
	if (abs(c) >= (rep / 2)) c = abs(c);
	return c;
}

float3 mod(float3 a, float3 b)
{
	return a - b * floor(a / b);
}

float scene(float3 p)
{
	p.x += time.x*0.5;
	float3 rep = float3(2., 0, 1.2);
	float3 p2 = mod(p, rep) - rep* .5;
	float3 size = float3(0.2,0.2,0.2);
	float d = length(max(abs(p2) - size, 0.0));
	return min(d - .02, min(length(p2 + float3(.5, 0, 0)) - .2, p.y + 0.2));
}

float3 getNormal(float3 p)
{
	float2 e = float2(0.0, 0.001);
	return normalize(scene(p) - float3(
		scene(p - e.yxx),
		scene(p - e.xyx),
		scene(p - e.xxy)
	));
}

float opShadow(in float3 ro, in float3 rd, in float mint, in float tmax)
{
	float res = 1.0;
	float t = mint;
	for (int i = 0; i<128; i++)
	{
		float h = scene(ro + rd * t);
		res = min(res, 2.0*h / t);
		t += clamp(h, 0.02, 0.10);
		if (h<0.001 || t>tmax) break;
	}
	return clamp(res, 0.0, 1.0);
}

float opAO(in float3 p, in float3 n)
{
	float value = 0.0;
	float s = 1.0;
	for (int i = 0; i < 3; ++i)
	{
		float stepSize = 0.13;
		float dist = scene(p + n * stepSize);
		value += (stepSize - dist) * s;
		s *= 0.7;
	}
	value = value;
	return clamp(sqrt((0.9 - value) * sqrt(1.0)), -1.0, 1.0);
}

float3 opSSS(in float3 ro2, in float3 rd2, in float3 matColor)
{
	const float kSSS = 0.1;
	float3 color = float3(0.0, 0.0, 0.0);
	for (int j = 0; j < 32; ++j)
	{
		float stepSize = kSSS;
		float d2 = scene(ro2 + rd2 * kSSS);
		if (d2 < 0.0)
		{
			break;
		}
		color += (matColor + (float3(1.0, 1.0, 1.0) * d2 * 0.1)) * 0.01 * d2;
	}
	return color;
}

float rayMarch(float3 ro, float3 rd)
{
	float t = 0.0;
	[loop]
	for (int i = 0; i < 1200; ++i)
	{
		float d = scene(ro + rd * t);
		if (d < 0.001)
		{
			return t;
		}
		t += d*.5;
		if (t > 100.0) return 100.0;
	}
	return 100.0;
}

[numthreads(8,8,1)]
void mainCS(uint3 threadId : SV_DispatchThreadID)
{
	uint index = threadId.x + threadId.y * uResolution.x + threadId.z * uResolution.x * uResolution.x;
	float2 pixelPos = float2(threadId.xy);
	float2 uv = (pixelPos / fResolution.xy * 2.0 - 1.0) * float2(fResolution.x / fResolution.y, -1.0);
	float3 color = float3(0.0, 0.0, 0.0);
	float3 ro = float3(0.0, 0.7 + sin(time.x*0.6) * .5, -4.0);
	float3 rd = normalize(float3(uv, 1.0));
	float3 l = normalize(float3(0.5, 1.0, 1.));
	float3 h = normalize(-rd + l);
	float3 matColor = float3(0.8, 0.3, 0.0);
	float mx = sin(time.x * 0.5) * .5;

	ro.xz = mul(ro.xz, rot(mx));
	rd.xz = mul(rd.xz, rot(mx));

	float r = rayMarch(ro, rd);
	if (r < 100.0)
	{
		float3 p = ro + rd * r;
		float3 n = getNormal(p);
		float shadow = opShadow(p, l, 0.1, 0.5);
		color =
			float3(0.2, 0.1, 0.1) +
			float3(0.4, 0.38, 0.37) * max(0.0, dot(n, l) * shadow) +
			float3(0.15, 0.15, 0.15) * pow(max(0.0, dot(n, h) * shadow), 32.)
			;

		color *= clamp(opAO(p,n), 0.0, 1.0);
		color += opSSS(p + l * 0.1, l, matColor);
		color *= pow(1.0-r/100.0, 3.9);
	}

	tex[threadId.xy] = float4(pow(color, 1.0 / 1.2), 1.0);
}