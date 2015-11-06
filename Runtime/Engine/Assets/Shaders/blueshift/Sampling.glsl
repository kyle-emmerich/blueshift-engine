// shadertype=glsl
vec3 TangentToWorld(vec3 dir, vec3 tangent_z) {
	vec3 up = abs(tangent_z.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent_x = normalize(cross(up, tangent_z));
	vec3 tangent_y = cross(tangent_z, tangent_x);
	return tangent_x * dir.x + tangent_y * dir.y + tangent_z * dir.z;
}

float ReverseBits(uint bits) {
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
 }

 
vec2 Hammersley(uint i, uint N, vec2 Random) {
     float E1 = 0.0;//fract((float)i / (float)N + float(Random.x & 0xffff) / (float)(1 << 16));
	 float E2 = 0.0;//float(ReverseBits(i) ^ Random.y) * 2.3283064365386963e-10;
	 return vec2(E1, E2);
}


vec4 UniformSampleHemisphere(vec2 E) {
	float phi = 2.0 * PI * E.x;
	float cos_T = E.y;
	float sin_T = sqrt(1.0 - cos_T * cos_T);

	vec3 H;
	H.x = sin_T * cos(phi);
	H.y = sin_T * sin(phi);
	H.z = cos_T;

	float PDF = 1.0 / (2.0 * PI);
	return vec4(H, PDF);
}

vec4 ImportanceSampleGGX(vec2 E, float roughness) {
	float m = roughness * roughness;
	float m2 = m * m;

	float phi = 2.0 * PI * E.x;
	float cos_T = sqrt((1.0 - E.y) / (1.0 + (m2 - 1.0) * E.y));
	float sin_T = sqrt(1.0 - cos_T * cos_T);

	vec3 H;
	H.x = sin_T * cos(phi);
	H.y = sin_T * sin(phi);
	H.z = cos_T;

	float d = (cos_T * m2 - cos_T) * cos_T + 1.0;
	float D = m2 / (PI * d * d);
	float PDF = D * cos_T;
	return vec4(H, PDF);
}

float MISWeight(uint n1, float PDF1, uint n2, float PDF2) {
	float weight1 = n1 * PDF1;
	float weight2 = n2 * PDF2;
	return weight1 * weight1 / (weight1 * weight1 + weight2 * weight2);
}