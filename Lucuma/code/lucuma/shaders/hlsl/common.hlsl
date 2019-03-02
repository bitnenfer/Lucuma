#define PI 3.14159265359

struct Material
{
	float3 albedo;
	float roughness;
	float metallic;
};

struct DirLight
{
	float3 color;
	float3 direction;
};

struct PointLight
{
	float3 color;
	float3 position;
	float radius;
};

float NormalDistributionFunctionGGXTR(float3 n, float3 m, float a)
{
	float a2 = a * a;
	float NdotM = max(0.0, dot(n, m));
	float NdotM2 = NdotM * NdotM;
	float denom = (NdotM * (a2 - 1.0) + 1.0);
	denom = PI * (denom * denom);
	return a2 / denom;
}

float GeometryGGX(float NdotV, float a)
{
	float k = (a + 1.0) * (a + 1.0) / 8.0;
	return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(float NdotV, float NdotL, float a)
{
	float g1 = GeometryGGX(NdotV, a);
	float g2 = GeometryGGX(NdotL, a);
	return g1 * g2;
}

float3 FresnelSchlick(float3 v, float3 h, float3 F0)
{
	float VdotH = max(0.0, dot(v, h));
	return F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
}

float3 ShaderUntexturedPBR(float3 n, float3 v, DirLight l, Material material)
{
	float3 F0 = lerp(float3(0.04, 0.04, 0.04), material.albedo, material.metallic);
	float3 h = normalize(v + l.direction);
	float roughness = material.roughness * material.roughness;
	float NdotL = max(0.0, dot(n, l.direction));
	float NdotV = max(0.0, dot(n, v));
	float3 Kd = (1.0 - F) * (1.0 - material.metallic);
	float3 radiance = l.color;
	float D = NormalDistributionFunctionGGXTR(n, h, roughness);
	float3 F = FresnelSchlick(v, h, F0);
	float G = GeometrySmith(NdotV, NdotL, roughness);
	float3 num = D * F * G;
	float denom = 4.0 * NdotL * NdotV;
	float3 specularBRDF = num / max(denom, 0.001);
	return (material.albedo * 0.03) + ((Kd * material.albedo / PI + specularBRDF) * radiance * NdotL);
}