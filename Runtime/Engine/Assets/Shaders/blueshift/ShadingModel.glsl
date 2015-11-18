#ifndef BLUESHIFT_SHADINGMODEL_H
#define BLUESHIFT_SHADINGMODEL_H
#include "Blueshift/Utility.glsl"

struct Fragment {
	vec2 texcoord;
	vec2 mat_texcoord;

	vec4 in_albedo;
	vec3 in_specular;
	float occlusion;
	vec3 in_normal;
	vec3 in_emissive;

	float roughness;
	float metalness;
	uint material_id;

	vec4 albedo;
	vec3 specular;
	vec3 fresnel;
	float specular_occlusion;

	vec3 radiance;
	vec3 irradiance;
	vec3 indirect_diffuse;
	vec3 indirect_specular;

	vec3 direct_diffuse;
	vec3 direct_specular;

	vec3 total_diffuse;
	vec3 total_specular;
	vec4 out_color;
};

float CalculateSpecularOcclusion(float NdV, float AO, float Roughness) {
    return clamp(pow(NdV + AO, Roughness) - 1.0 + AO, 0.0, 1.0);
}

void SampleMaps(inout Fragment f, in sampler2D albedo, in sampler2D specular, in sampler2D material, in sampler2D normal, in sampler2D emissive, in sampler3D material_lookup) {
	vec4 albedo = ToLinear(texture(albedo, f.texcoord));
	vec4 spec_occlusion = ToLinear(texture(specular, f.texcoord));
	vec4 material = texture(material, f.texcoord);
	vec4 normal = texture(normal, f.texcoord);
	vec4 emissive = ToLinear(emissive, f.texcoord);
	
	float obj_roughness = material.r;
	float obj_metalness = material.g;
	f.material_id = (uint)floor(material.b * 255.0);

	vec4 mat_map = ToLinear(
	f.roughness = obj_roughness * mat_map.r;
	f.metalness = obj_roughness * mat_map.g;
	
}

void Integrate(inout Fragment f) {
	f.total_diffuse = (f.direct_diffuse + f.indirect_diffuse) * f.in_occlusion;
	f.total_specular = (f.direct_specular + f.indirect_specular) * f.specular_occlusion;
	f.out_color = vec4(ToGamma(f.total_diffuse + f.total_specular + f.in_emissive), f.albedo.w);
}

#endif