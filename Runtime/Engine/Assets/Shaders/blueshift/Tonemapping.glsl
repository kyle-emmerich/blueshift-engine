#ifndef BLUESHIFT_TONEMAPPING_H
#define BLUESHIFT_TONEMAPPING_H
#include "Blueshift/Utility.glsl"

vec3 TonemapReinhard(vec3 rgb) {
	return ToGamma(rgb / (rgb + vec3_splat(1.0));
}
vec4 TonemapReinhard(vec4 rgba) {
	return vec4(TonemapReinhard(rgba.xyz), rgba.w);
}

vec3 TonemapFilmic(vec3 rgb) {
	rgb = max(vec3_splat(0.0), rgb - 0.004);
	rgb = (rgb * (6.2 * rgb + 0.5)) / (rgb * (6.2 * rgb + 1.7) + 0.06);
	return rgb;
}
vec4 TonemapFilmic(vec4 rgba) {
	return vec4(TonemapReinhard(rgba.xyz), rgba.w);
}


#endif


