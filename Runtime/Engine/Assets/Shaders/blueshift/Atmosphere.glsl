#ifndef BLUESHIFT_ATMOSPHERE_H
#define BLUESHIFT_ATMOSPHERE_H

#include "Blueshift/Utility.glsl"

//See http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter16.html
//This file contains some of the functions involved in atmospheric scattering
//It does not, however, provide an optimized implementation at this time.

//The phase function for g =/= 0
//Purpose:
//	Describes the fraction of light scattered toward the direction of the camera
//	as a function of the angle q. A constant g controls the symmetry of the scattering.
//	Implemented as described by Sean O'Neil.
//Parameters:
//	cosine_q	The cosine of the angle between the two involved rays
//				(the ray to the point rendered vs the light-ray)
//	g			The symmetry of the scattering; used only for Mie scattering.
//				Depends on aerosols in air; calculated on CPU
float MiePhase(float cosine_q, float g) {
	float g2 = g * g;
	float F_1 = (3 * (1 - g2)) / (2 * (2 + g2));
	float F_2_num = 1 + cosine_q * cosine_q;
	float F_2_den = pow(1 + g2 - 2 * g * cosine_q, 1.5);
	float F_2 = F_2_num / F_2_den;
	return F_1 * F_2;
}
//The phase function for g=0
//Purpose:
//	Describes the fraction of light scattered toward the direction of the camera
//	as a function of the angle q. Simplified version of above function when g=0
//Parameters:
//	cosine_q	The cosine of the angle between the two involved rays.
//				(the ray to the point 
float RayleighPhase(float cosine_q) {
	//The left term simplifies to 3/4, the right term simplifies to have a denominator of 1^(3/2)=1
	//Super fast.
	float F_1 = 0.75;
	return F_1 * (1 + cosine_q * cosine_q);
}

//A small structure describing a planet and its atmosphere on a high level.
struct Planet {
	float radius;
	float thickness;
	float density_ratio;

	float K_r;
	float K_m;
};
//A small structure describing a star and its light.
struct Light_Star {
	vec3 light_dir;
	vec3 wavelengths;
};

//The amount of light scattered away by the atmosphere for a unit length
//Parameters:
//	h		The height fraction; 0.0 is sea level, 1.0 is the top of the atmosphere
//	planet	The planet whose atmosphere is scattering the light
float DensityRatio(float h, Planet planet) {
	return exp(-h / planet.density_ratio);
}
 
vec3 IntegrateScattering(
		in vec3 start, in vec3 ray, in float far, 
		in vec3 light_dir, in float beta,
		in float K_r_4PI, in float K_m_4PI,
		in uint num_samples,
		Planet planet) {
	float delta_s = far / num_samples;
	float scaled_delta_s = delta_s * scale;
	vec3 ds = ray * delta_s;
	vec3 s = start + ds * 0.5;

	vec3 integrated = vec3_splat(0.0);
	for (uint i = 0; i < num_samples; i++) {
		float height = length(s_p);
		float depth = exp(density_scaled * (inner_radius - height));
		float cos_l = dot(light_dir, s_p) / height;
		float cos_v = dot(ray, s_p) / height;

		float scatter = start_offset + depth * (DensityRatio(cos_l, planet) * DensityRatio(cos_v, planet));
		float attenuation = exp(-scatter * beta); 
		
		integrated += attenuation * (depth * scaled_delta_s);

		s += ds;
	}
	return integrated;
}

//This function assumes that all positions are relative to the center of the planet.
void SkyFromSpaceScattering(
	in Planet planet,
	in Light_Star light,
	in vec3 camera_pos, in vec3 end_pos,
	in uint num_samples,
	out vec3 rayleigh, out vec3 mie) {
	float thickness = planet.thickness;
	float outer_radius = thickness + planet.radius;
	float inner_radius = planet.radius;
	float camera_height = length(camera_pos) - inner_radius;

	vec3 ray = end_pos - camera_pos;
	float far = length(ray);
	ray /= far;

	//Calculate the nearest intersection of the ray and atmosphere
	float B = 2.0 * dot(camera_pos, ray);
	float C = camera_height - (outer_radius * outer_radius);
	float determinant = max(0.0, B * B - 4.0 * C);
	float near = 0.5 * (-B - sqrt(determinant));

	//Adjust the ray starting position to where the atmosphere starts
	vec3 start = camera_pos + ray * near;
	far -= near;
	float start_cos = dot(ray, start) / outer_radius;
	float start_depth = DensityRatio(1.0, planet);
	float start_offset = start_depth * DensityRatio(start_cos, planet);

	float scale = 1.0 / thickness;
	float density_scaled = scale / H_0;

	float K_r_4PI = planet.K_r * 4.0 * PI;
	float K_m_4PI = planet.K_m * 4.0 * PI;

	vec3 inv_wavelength = rcp(light.wavelengths);
	vec3 beta = (K_r_4PI + K_m_4PI) * inv_wavelength;

	vec3 integrated = IntegrateScattering(start, ray, far, light.direction, beta, K_r_4PI, K_m_4PI, num_samples, planet);

	//Scale the scattered light and then output.
	rayleigh = integrated * inv_wavelength * K_r;
	mie = integrated * K_m;
}
void SkyFromAtmosphereScattering(
	in Planet planet,
	in Light_Star light,
	in vec3 camera_pos, in vec3 end_pos,
	in uint num_samples,
	out vec3 rayleigh, out vec3 mie) {
	float thickness = planet.thickness;
	float outer_radius = thickness + planet.radius;
	float inner_radius = planet.radius;
	float camera_height = length(camera_pos) - inner_radius;

	vec3 start = camera_pos;
	float start_cos = dot(ray, start) / outer_radius;
	float start_depth = DensityRatio(1.0, planet);
	float start_offset = start_depth * DensityRatio(start_cos, planet);

	float scale = 1.0 / thickness;
	float density_scaled = scale / H_0;

	float K_r_4PI = planet.K_r * 4.0 * PI;
	float K_m_4PI = planet.K_m * 4.0 * PI;

	vec3 inv_wavelength = rcp(light.wavelengths);
	vec3 beta = (K_r_4PI + K_m_4PI) * inv_wavelength;

	vec3 integrated = IntegrateScattering(start, ray, far, light.direction, beta, K_r_4PI, K_m_4PI, num_samples, planet);

	//Scale the scattered light and then output.
	rayleigh = integrated * inv_wavelength * K_r;
	mie = integrated * K_m;
}
	

vec3 CombineScattering(vec3 rayleigh, vec3 mie, float cosine_q, float g) {
	float cos_2 = cosine_q * cosine_q;
	return RayleighPhase(cosine_q) * rayleigh + MiePhase(cosine_q, g) * mie;
}

#endif