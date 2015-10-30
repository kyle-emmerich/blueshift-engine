float PI = 3.14159265359;

vec3 DiffuseLambert(vec3 c) {
	return (c / 3.14159).xyz;
}

vec3 FresnelSchlick(float cos_T, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cos_T, 5.0);
}

float flip(float v) {
    return v > 0 ? 1 : 0;
}

float GGXDistribution(vec3 N, vec3 H, float roughness) {
	float NdH = dot(N, H);
	float roughness2 = roughness * roughness;
	float NdH2 = NdH * NdH;
	float den = NdH2 * roughness2 + (1 - NdH2);
	return (flip(NdH) * roughness2) / (PI * den * den);
}

float GGXPartialGeometryTerm(vec3 V, vec3 N, vec3 H, float roughness) {
    float VdH2 = clamp(dot(V,H), 0.0, 1.0);
    float chi = flip( VdH2 / clamp(dot(V, N), 0.0, 1.0));
    VdH2 = VdH2 * VdH2;
    float tan2 = (1 - VdH2) / VdH2;
    return (chi * 2) / (1 + sqrt(1 + roughness * roughness * tan2));
}