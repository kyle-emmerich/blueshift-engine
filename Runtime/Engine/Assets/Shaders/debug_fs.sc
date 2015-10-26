$input v_worldpos, v_normal

#include "bgfx_shader.sh"

float K = 0.7978845608;
float pi = 3.14159265359;
float inv_pi = 1.0 / 3.14159265359;

vec3 DiffuseLambert(vec3 color) {
	return color * inv_pi;
}
vec3 DiffuseBurley(vec3 color, float roughness, float NdV, float NdL, float VdH) {
	float FD90 = 0.5 + (2.0 * VdH * VdH * roughness);
	float FdV = 1.0 + (FD90 - 1.0) * pow(1.0 - NdV, 5.0);
	float FdL = 1.0 + (FD90 - 1.0) * pow(1.0 - NdL, 5.0);
	return color * (inv_pi * FdV * FdL);
}
vec3 DiffuseOrenNayar(vec3 color, float roughness, float NdV, float NdL, float VdH) {
	float a = roughness * roughness;
	float s = a * a;
	float VdL = 2.0 * VdH * VdH - 1.0;
	float cos_ri = VdL - NdV * NdL;
	float c1 = 1.0 - (0.5 * s) / (s + 0.33);
	float c2 = 0.45 * s / (s + 0.09) * cos_ri * (cos_ri >= 0 ? 1.0 / max(NdL, NdV) : 1.0);
	return color * inv_pi * (c1 + c2) * (1 + roughness * 0.5);
}

vec3 Fresnel_Schlick(vec3 specular_color, float VdH) {
	vec3 spec_sqrt = sqrt(clamp(vec3(0.0, 0.0, 0.0), vec3(0.99, 0.99, 0.99), specular_color));
	vec3 n = (1 + spec_sqrt) / (1 - spec_sqrt);
	vec3 g = sqrt(n * n + VdH * VdH - 1.0);

	vec3 g_VdH = (g - VdH) / (g + VdH);
	vec3 g_VdH_1 = ((g + VdH) * VdH - 1.0) / ((g - VdH) * VdH + 1.0);

	return 0.5 * g_VdH * g_VdH * (1 + g_VdH_1 * g_VdH_1);
}

float G_Implicit(float NdL, float NdV) {
	return NdL * NdV;
}
float G_Smith(float LdH, float VdH, float roughness) {
	float k = roughness * K;
	float G_LdH = LdH / (LdH * (1 - k) + k);
	float G_VdH = VdH / (VdH * (1 - k) + k);
	return G_LdH * G_VdH;
}
float G_CookTorrance(float NdH, float NdV, float VdH, float NdL) {
	return min(1, min((2.0 * NdH * NdV) / VdH, (2.0 * NdH * NdL) / VdH));
}

float D_Beckmann(float NdH, float roughness) {
	float a = roughness * roughness;
	float NdH_2 = NdH * NdH;
	float power = (NdH_2 - 1) / (a * NdH_2);
	return exp(power) / (pi * a * NdH_2 * NdH_2);
}
float D_GGX(float NdH, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float d = (NdH * a2 - NdH) * NdH + 1.0;
	return a2 / (pi * d * d);
}
float D_GGXAnisotropic(float NdH, vec3 H, float roughness_x, float roughness_y, float x, float y) {
	float ax = roughness_x * roughness_x;
	float ay = roughness_y * roughness_y;
	float XdH = dot(x, H);
	float YdH = dot(y, H);
	float d = XdH * XdH / (ax * ax) + YdH * YdH / (ay * ay) + NdH * NdH;
	return 1.0 / (pi * ax * ay * d * d);
}

void main() {
	vec3 downlight = normalize(vec3(0.0, -1.0, -1.0));
	vec3 eye_pos = vec3(0.0, 0.0, -10.0);
	vec3 surface_pos = v_worldpos;

	//L = vector to light, just the inverse of a directional light
	vec3 L = downlight * -1.0;
	//the normal at the point
	vec3 N = v_normal;
	//V = vector to eye
	vec3 V = normalize(surface_pos - eye_pos);
	//H = the half-vector between L and V
	vec3 H = normalize(L + V);

	float NdL = max(0, dot(N, L));
	float NdV = max(1e-5, dot(N, V));
	float NdH = max(0, dot(N, H));
	float LdH = max(0, dot(L, H));
	float VdH = max(0, dot(V, H));

	vec3 diffuse = vec3(0.5, 0.5, 0.5);
	vec3 specular = vec3(1.0, 0.765557, 0.336057);
	float roughness = 0.02;

	vec3 F = Fresnel_Schlick(specular, VdH);
	float G = G_Smith(LdH, VdH, roughness);
	float D = D_GGX(NdH, roughness);

	vec3 color = DiffuseLambert(diffuse) + D * F;
	gl_FragColor = vec4(color, 1.0);
}	
