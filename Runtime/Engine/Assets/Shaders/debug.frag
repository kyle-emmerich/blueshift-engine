$input v_worldpos, v_normal

#include "bgfx_shader.sh"

float PI = 3.14159265359;
//int specular_samples = 20;
//int mips_count = 4;

uniform vec3 u_lightVec = vec3(1.0, 0.0, 0.0);
//uniform vec2 random[specular_samples];

vec3 DiffuseLambert(vec3 c) {
	return (c / 3.14159).xyz;
}

vec3 FresnelSchlick(float cos_T, float ior, vec3 color, float metallic) {
	vec3 F0 = abs((1.0 - ior) / (1.0 + ior));
	F0 = F0 * F0;
	F0 = lerp(F0, color.rgb, metallic);
	return F0 + (1.0 - F0) * pow(1.0 - cos_T, 5.0);
}

float GGXPartialGeometryTerm(vec3 V, vec3 N, vec3 H, float roughness) {
	return 0.0;
}

vec3 GenerateSampleVector(int i, int num_samples, float roughness) {
	//float theta_s = atan(roughness * sqrt(random[i].x));
	//float phi_s = 2 * pi * random[i].y;
	
	return vec3(0.0, 0.0, 0.0);
}
/*
vec3 GGXSpecular(SAMPLERCUBE environment, vec3 N, vec3 V, float roughness, out vec3 k_s) {
	vec3 reflection = reflect(V * -1.0, N);
	// idk what this is yet? mat3 world_frame = GenerateFrame(reflection);
	vec3 radiance = vec3(0.0, 0.0, 0.0);
	float NdV = max(0.0, min(1.0, dot(N, V)));
	
	for (int i = 0; i < specular_samples; ++i) {
		vec3 sample_vector = GenerateSampleVector(i, specular_samples, roughness);
		sample_vector = normalize(mul(u_world[0], sample_vector));

		vec3 half_vector = normalize(sample_vector + V);
		float cos_T = max(0.0, min(1.0, dot(sample_vector, normal)));
		float sin_T = sqrt(1.0 - cos_T * cos_T);

		vec3 fresnel_term = Fresnel_Schlick(max(0.0, min(1.0, dot(half_vector, V)), F0);
		float geometry = GGXPartialGeometryTerm(V, N, half_vector, roughness) * GGXPartialGeometryTerm(sample_vector, N, half_vector, roughness);
		
		float denominator = max(0.0, min(1.0, 4.0 * (NdV * max(0.0, min(1.0, half_vector, normal))) + 0.05));
		k_s += fresnel_term;
		radiance += texture(environment, sample_vector, (roughness * mips_count)).rgb * geometry * fresnel * sin_T / denominator;
	}
	k_s = max(0.0, min(1.0(k_s / specular_samples)));
	return radiance / specular_samples;
}*/

void main() {
	vec3 downlight = mul(normalize(u_lightVec.xyz), u_model[0]);
	vec3 eye_pos = vec3(0.0, 0.0, -2.0);
	vec3 surface_pos = v_worldpos;

	//L = vector to light, just the inverse of a directional light
	vec3 L = downlight * 1.0;
	//the normal at the point
	vec3 N = v_normal;

	float NdL = max(0, dot(N, L));

	//material values to be read in some time
	float metallic = 0.0;
	float k_s = 0.0; //specular weight
	float k_d = (1 - k_s) * (1 - metallic); //diffuse weight

	vec3 diffuse = DiffuseLambert(vec3(1.0, 0.7, 0.75)) * NdL;
	vec3 specular = vec3(0.0, 0.0, 0.0);//GGXSpecular( );

	vec3 color = diffuse;
	gl_FragColor = vec4(diffuse, 1.0);
}	
