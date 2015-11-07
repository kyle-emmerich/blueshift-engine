float PI = 3.14159265359;

float saturate(float x) {
	return clamp(x, 0.0, 1.0);
}

vec3 DiffuseLambert(vec3 c) {
	return (c / PI).xyz;
}

vec3 FresnelSchlick(float cos_T, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cos_T, 5.0);
}

float positive(float x) {
	return x > 0.0 ? 1.0 : 0.0;
}

float GGXDistribution(vec3 N, vec3 H, float roughness) {
	float NdH = dot(N, H);
	float roughness2 = roughness * roughness;
	float NdH2 = NdH * NdH;
	float denom = NdH2 * roughness2 + (1 - NdH2);
	return (positive(NdH) * roughness2) / (3.14159265359 * denom * denom);
}

float GGXPartialGeometryTerm(vec3 V, vec3 N, vec3 H, float roughness) {
    float VdH2 = saturate(dot(V,H));
	float VdN = saturate(dot(V, N));
    float chi = positive(VdH2 / VdN);
    VdH2 = VdH2 * VdH2;
    float tan2 = (1.0 - VdH2) / VdH2;
    return (chi * 2.0) / (1.0 + sqrt(1.0 + roughness * roughness * tan2));
}