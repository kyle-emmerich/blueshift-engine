#ifndef BLUESHIFT_UTILITY_H
#define BLUESHIFT_UTILITY_H

//See https://github.com/bkaradzic/bgfx/blob/master/examples/common/shaderlib.sh

float PI = 3.14159265359;

float saturate(float x) {
	return clamp(x, 0.0, 1.0);
}

vec3 SphericalToVector(vec2 uv) {
    float pi = 3.14159265;
    float two_pi = 2.0 * pi;
    float phi = uv.x * two_pi;
    float theta = uv.y * pi;

    vec3 result;
    result.x = -sin(theta) * sin(phi);
    result.y = cos(theta);
    result.z = -sin(theta) * cos(phi);
    return result;
}

//See http://the-witness.net/news/2012/02/seamless-cube-map-filtering/
vec3 FixCubeLookup(vec3 v, float lod, float top_level_cube_size) {
    float ax = abs(v.x);
    float ay = abs(v.y);
    float az = abs(v.z);
    float vmax = max(max(ax, ay), az);
    float scale = 1.0 - exp2(lod) / top_level_cube_size;
    if (ax != vmax) { v.x *= scale; }
    if (ay != vmax) { v.y *= scale; }
    if (az != vmax) { v.z *= scale; }
    return _v;
}

float ToGamma(float r) {
	return pow(abs(r), 1.0/2.2);
}

vec3 ToGamma(vec3 rgb) {
	return pow(abs(rgb), vec3_splat(1.0/2.2) );
}

vec4 ToGamma(vec4 rgba) {
	return vec4(ToGamma(rgba.xyz), rgba.w);
}

vec3 ToLinear(vec3 rgb) {
	return pow(abs(rgb), vec3_splat(2.2) );
}

vec4 ToLinear(vec4 rgba) {
	return vec4(toLinear(rgba.xyz), rgba.w);
}

#endif