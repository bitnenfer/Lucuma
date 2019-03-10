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

float3 ShaderUntexturedDirLightPBR(float3 n, float3 v, DirLight l, Material material)
{
	float3 F0 = lerp(float3(0.04, 0.04, 0.04), material.albedo, material.metallic);
	float3 h = normalize(v + l.direction);
	float roughness = material.roughness * material.roughness;
	float NdotL = max(0.0, dot(n, l.direction));
	float NdotV = max(0.0, dot(n, v));
	float3 radiance = l.color;
	float D = NormalDistributionFunctionGGXTR(n, h, roughness);
	float3 F = FresnelSchlick(v, h, F0);
	float G = GeometrySmith(NdotV, NdotL, roughness);
	float3 num = D * F * G;
	float3 Kd = (1.0 - F) * (1.0 - material.metallic);
	float denom = 4.0 * NdotL * NdotV;
	float3 specularBRDF = num / max(denom, 0.001);
	return (material.albedo * 0.03) + ((Kd * material.albedo / PI + specularBRDF) * radiance * NdotL);
}

float3 ShaderUntexturedPointLightPBR(float3 p, float3 n, float3 v, PointLight l, Material material)
{
	float3 dir = normalize(l.position - p);
	float dist = length(l.position - p);
	float attenuation = clamp(1.0 - dist * dist, 0.0, 1.0);
	float3 F0 = lerp(float3(0.04, 0.04, 0.04), material.albedo, material.metallic);
	float3 h = normalize(v + dir);
	float roughness = material.roughness * material.roughness;
	float NdotL = max(0.0, dot(n, dir));
	float NdotV = max(0.0, dot(n, v));
	float3 radiance = l.color * attenuation;
	float D = NormalDistributionFunctionGGXTR(n, h, roughness);
	float3 F = FresnelSchlick(v, h, F0);
	float G = GeometrySmith(NdotV, NdotL, roughness);
	float3 num = D * F * G;
	float3 Kd = (1.0 - F) * (1.0 - material.metallic);
	float denom = 4.0 * NdotL * NdotV;
	float3 specularBRDF = num / max(denom, 0.001);
	return (material.albedo * 0.03) + ((Kd * material.albedo / PI + specularBRDF) * radiance * NdotL);
}

float3 ShadeBlinnPhong(float3 n, float3 v, DirLight l, Material material)
{
	float3 lightColor = clamp(l.color * 0.1, 0.0, 1.0);
	float3 h = normalize(v + normalize(l.direction));
	float NdotL = max(0.0, dot(n, l.direction));
	float NdotH = max(0.0, dot(n, h));
	
	float3 diffuse = NdotL * ((lightColor + material.albedo) / 2.5);
	float3 specular = pow(NdotH, max(1.0 - material.roughness, 0.1) * 128.0) * float3(1.0, 1.0, 1.0);
	float3 ambient = ((lightColor + material.albedo) / 2.5) * 0.1;

	return diffuse + specular + ambient;
}

//-----

cbuffer ConstantBuffer : register(b0)
{
	float4x4 MVP;
	float4x4 invModel;
	float4x4 model;
	float4x4 view;
	float4x4 projection;
	float4 cameraPosition;

};

cbuffer MaterialBuffer : register(b1)
{
	float4 roughnessMetallic;
	float4 albedo;
	float4 lightPos;
	float4 lightColorRadius;
};

struct VertexInput
{
	float3 position : TEXCOORD0;
	float2 texCoord : TEXCOORD2;
	float3 normal : TEXCOORD1;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float3 normal: TEXCOORD3;
	float3 view : TEXCOORD4;
	float4 worldPosition : TEXCOORD5;
};

PixelInput DefaultVS(VertexInput input)
{
	PixelInput output;

	output.position = mul(MVP, float4(input.position, 1.0));
	output.normal = mul(invModel, float4(input.normal, 0.0)).xyz;
	output.view = normalize(cameraPosition - mul(model, float4(input.position, 1.0))).xyz;
	output.worldPosition = mul(model, float4(input.position, 1.0));

	return output;
}

float4 DefaultPS(PixelInput input) : SV_TARGET
{
	float3 V = normalize(input.view);
	float3 N = normalize(input.normal);
	float3 P = input.worldPosition.xyz;

	Material material;
	material.albedo = albedo.rgb;
	material.roughness = roughnessMetallic.x;
	material.metallic = roughnessMetallic.y;

	DirLight dirLights[2];
	PointLight pointLights[2];

	dirLights[0].color = float3(4.0, 4.0, 4.0);
	dirLights[0].direction = normalize(float3(0.8, 1.0, 0.5));

	dirLights[1].color = float3(1.0, 0.5, 0.5);
	dirLights[1].direction = normalize(float3(-0.8, -1.0, -0.5));

	pointLights[0].position = lightPos.xyz;
	pointLights[0].radius = lightColorRadius.w;
	pointLights[0].color = lightColorRadius.xyz;

	float3 color = float3(0.0, 0.0, 0.0);
	
	for (int i = 0; i < 2; ++i)
	{
		color += ShaderUntexturedDirLightPBR(N, V, dirLights[i], material);
	}

	for (int i = 0; i < 1; ++i)
	{
		color += ShaderUntexturedPointLightPBR(P, N, V, pointLights[i], material);
	}

	color = color / (color + 1.0);
	color = pow(color, 1.0 / 2.2);

	return float4(color, 1.0);
}
