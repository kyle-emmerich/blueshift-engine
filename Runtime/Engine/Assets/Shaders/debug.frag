$input v_worldpos, v_normal

#include "bgfx_shader.sh"
#include "blueshift/BRDF.glsl"
#include "blueshift/Sampling.glsl"

const int specular_samples = 2;
int mips_count = 1;

uniform float u_samples = 2.0;
uniform vec3 u_lightPos = vec3(0.0, 0.0, 1.0);
SAMPLERCUBE(u_texture, 0);
//uniform vec2 random[specular_samples];

mat3 GenerateFrame(vec3 norm) {
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(norm, up);
	up = cross(right, norm);
	right = cross(norm, up);

	mat3 frame;
	frame[0] = right;
	frame[1] = up;
	frame[2] = norm;
	return frame;
}

vec3 GGXSpecular(in BgfxSamplerCube environment, in vec3 L, in vec3 N, in vec3 V, in float ior, in vec3 color, in float metallic, in float roughness, inout vec3 k_s) {
	vec3 reflection = reflect(V * -1.0, N);
	mat3 world_frame = GenerateFrame(reflection);
	vec3 radiance = vec3(0.0, 0.0, 0.0);
	float NdV = clamp(dot(N, V), 0.0, 1.0);

	vec3 F0 = abs((1.0 - ior) / (1.0 + ior));
	F0 = F0 * F0;
	F0 = lerp(F0, color.rgb, metallic);

	vec3 weight = vec3(0.0, 0.0, 0.0);

	int samples = (int)u_samples;
	for (int i = 0; i < 10; i++) {
	//fix this?
		vec3 sample_vector = L;//vec3(0.0, 1.0, 0.0);//GenerateSampleVector( roughness);
		sample_vector = normalize(vec4(sample_vector, 1.0).xyz);

		vec3 half_vector = normalize(sample_vector + V);
		float cos_T = clamp(dot(sample_vector, N), 0.0, 1.0);
		float sin_T = sqrt(1.0 - cos_T * cos_T);

		vec3 fresnel_term = FresnelSchlick(clamp(dot(half_vector, V), 0.0, 1.0), F0);
		float geometry = GGXPartialGeometryTerm(V, N, half_vector, roughness) * GGXPartialGeometryTerm(sample_vector, N, half_vector, roughness);
		
		float denominator = clamp(4.0 * (NdV * clamp(dot(half_vector, N), 0.0, 1.0) + 0.05), 0.0, 1.0);
		weight += fresnel_term;
		radiance += pow(textureCubeLod(environment, sample_vector, roughness * 12.0).rgb, 2.0) * geometry * fresnel_term * sin_T / denominator;
	}
	k_s = clamp(weight / 10, 0.0, 1.0);
	return clamp(radiance / 10, 0.0, 1.0);
}

void main() {
	vec3 eye_pos = transpose(u_invView)[3].xyz;
	vec3 surface_pos = v_worldpos.xyz + transpose(u_model[0])[3].xyz;

	vec3 L = normalize(mul(u_view, u_lightPos) - surface_pos);
	//the normal at the point
	vec3 N = normalize(v_normal);

	vec3 V = normalize(-v_worldpos.xyz);
	vec3 H = normalize(V + L);

	float NdL = max(0, dot(N, L));
	float NdV = max(0, dot(N, V));
	float NdH = max(0, dot(N, H));
	float VdH = max(0, dot(V, H));

	//material values to be read in some time
	vec3 albedo = vec3(0.56, 0.57, 0.58);
	float metallic = 0.9;
	float ior = 1.2;
	float roughness = 0.35;

	vec3 k_s = 0.0; //specular weight
	vec3 specular = GGXSpecular(u_texture, L, N, V, ior, albedo, metallic, roughness, k_s);
	vec3 k_d = (1 - k_s) * (1 - metallic); //diffuse weight
	
	vec3 irradiance = vec3(1.0, 1.0, 1.0) * textureCubeLod(u_texture, N, roughness * 12.0) * NdL;
	vec3 diffuse = albedo * NdL * 0.5;

	vec3 color = k_d * diffuse + specular;
	//color = vec3(1.0, 1.0, 1.0);
	//dot(L, N)*diffuse + pow(NdH, 4.0) 
	float distribution = GGXDistribution(N, H, roughness);
	float geometry =
			GGXPartialGeometryTerm(V, N, H, roughness) * 
			GGXPartialGeometryTerm(L, N, H, roughness);
	vec3 F0 = abs((1.0 - ior) / (1.0 + ior));
	F0 = F0 * F0;
	F0 = lerp(F0, albedo, metallic);
	vec3 fresnel = FresnelSchlick(VdH, F0);

	vec3 cook_torrance = (fresnel * distribution * geometry) / (4 * NdV * NdL);

	gl_FragColor = vec4((k_s * cook_torrance) + (k_d * diffuse), 1.0);
	//gl_FragColor = vec4(((NdL * albedo)) + pow(NdH * NdL, 4.0), 1.0);
}	
