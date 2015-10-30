$input v_worldpos, v_normal

#include "bgfx_shader.sh"
#include "blueshift/BRDF.glsl"
#include "blueshift/Sampling.glsl"

float PI = 3.14159265359;
const int specular_samples = 2;
int mips_count = 1;

uniform float u_samples = 2.0;
uniform vec3 u_lightVec = vec3(0.0, 0.0, 1.0);
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

vec3 GGXSpecular(in BgfxSamplerCube environment, in vec3 N, in vec3 V, in float ior, in vec3 color, in float metallic, in float roughness, inout vec3 k_s) {
	vec3 reflection = reflect(V * -1.0, N);
	mat3 world_frame = GenerateFrame(reflection);
	vec3 radiance = vec3(0.0, 0.0, 0.0);
	float NdV = clamp(dot(N, V), 0.0, 1.0);

	vec3 F0 = abs((1.0 - ior) / (1.0 + ior));
	F0 = F0 * F0;
	F0 = lerp(F0, color.rgb, metallic);

	vec3 weight = vec3(0.0, 0.0, 0.0);

	int samples = (int)u_samples;
	for (int i = 0; i < 20; i++) {
	//fix this?
		vec3 sample_vector = GenerateSampleVector(Hammersley(i, 20), roughness);
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
	k_s = clamp(weight / 20, 0.0, 1.0);
	return clamp(radiance / 20, 0.0, 1.0);
}

void main() {
	vec3 downlight = normalize(u_lightVec).xyz;
	vec3 eye_pos = mul(u_model[0], u_view[0].xyz);
	vec3 surface_pos = gl_FragCoord.xyz / gl_FragCoord.w;

	//L = vector to light, just the inverse of a directional light
	vec3 L = downlight * -1.0;
	//the normal at the point
	vec3 N = v_normal;

	vec3 V = normalize(surface_pos - eye_pos);

	float NdL = max(0, dot(N, L));

	//material values to be read in some time
	vec3 albedo = vec3(1.0, 1.0, 1.0);
	float metallic = 0.7;
	float ior = 1.7;
	float roughness = 0;
	vec3 k_s = 0.0; //specular weight
	vec3 specular = GGXSpecular(u_texture, N, V, ior, albedo, metallic, roughness, k_s);
	vec3 k_d = (1 - k_s) * (1 - metallic); //diffuse weight
	
	vec3 irradiance = vec3(1.0, 1.0, 1.0) * textureCubeLod(u_texture, N, 8) * NdL;
	vec3 diffuse = albedo * irradiance;

	vec3 color = k_d * diffuse + specular;
	gl_FragColor = vec4(color, 1.0);
}	
