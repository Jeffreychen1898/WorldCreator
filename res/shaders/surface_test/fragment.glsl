#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out uint Id;

in vec3 v_position;
in vec3 v_normal;
in float v_gamma;
in float v_exposure;
in float v_intensity;
in vec3 v_sunColor;
in vec3 v_sunPosition;
in vec3 v_cameraPosition;

#define PI 3.1415926535
#define EP 0.0000000001

float DistributionFunc(float roughness, vec3 normal, vec3 halfway)
{
	float NdotH_sq = pow(max(dot(normal, halfway), 0.0), 2.0);
	float rough_sq = pow(roughness, 2.0);
	float denominator = max(PI * pow(NdotH_sq * (rough_sq - 1.0) + 1.0, 2.0), EP);
	
	return rough_sq / denominator;
}

float GeoShadowFunc(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness)
{
	float NdotV = max(dot(normal, viewDir), 0.0);
	float NdotL = max(dot(normal, lightDir), 0.0);
	float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
	
	float GeoFunc1 = NdotV / max(NdotV * (1.0 - k) + k, EP);
	float GeoFunc2 = NdotL / max(NdotL * (1.0 - k) + k, EP);
	
	return GeoFunc1 * GeoFunc2;
}

vec3 FresnelFunc(vec3 F0, vec3 normal, vec3 halfway)
{
	vec3 ones = vec3(1.0);
	return F0 + (ones - F0) * pow(1.0 - dot(normal, halfway), 5.0);
}

vec3 PDR(vec3 textureFragment, vec3 normal, vec3 viewDir, vec3 lightDir, vec3 halfway)
{
	// randomly set the values for now
	float roughness = 0.2;
	vec3 reflectivity = vec3(0.9);
	float metallic = 1.0;
	// end of values
	
	vec3 F0 = mix(vec3(0.04), textureFragment, metallic);
	vec3 fresnel = FresnelFunc(reflectivity, normal, halfway);
	float distribution = DistributionFunc(roughness, normal, halfway);
	float geometryShadowing = GeoShadowFunc(normal, viewDir, lightDir, roughness);
	
	float VdotN = max(dot(viewDir, normal), 0.0);
	float LdotN = max(dot(lightDir, normal), 0.0);
	
	vec3 cookTorrenceNum = fresnel * geometryShadowing * distribution;
	float cookTorrenceDenom = max(4.0 * VdotN * LdotN, EP);
	
	vec3 cookTorrence = cookTorrenceNum / cookTorrenceDenom;
	vec3 Kd = (vec3(1.0) - fresnel) * (1.0 - metallic);
	float NdotL = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = (Kd * textureFragment / PI + cookTorrence) * NdotL;
	
	return diffuse;
}

void main()
{
	vec3 albedo = vec3(0.3);
	vec3 view_dir = normalize(v_cameraPosition - v_position);
	vec3 light_dir = normalize(v_sunPosition - v_position);
	vec3 halfway = (view_dir + light_dir) / 2.f;
	
	vec3 diffuse_lighting = PDR(albedo, v_normal, view_dir, light_dir, halfway) * v_sunColor * v_intensity;
	vec3 ambient_lighting = vec3(0.01) * v_sunColor;
	
	vec3 result_color = diffuse_lighting + ambient_lighting;
	
	vec3 tone_mapped = vec3(1.0) - exp(-result_color * v_exposure);
	
	vec3 gamma_corrected = pow(tone_mapped, vec3(1.0 / v_gamma));
	
	FragColor = vec4(gamma_corrected, 1.0);
	Id = 1u;
}
