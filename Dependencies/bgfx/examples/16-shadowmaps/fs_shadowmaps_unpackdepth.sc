$input v_texcoord0

/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "../common/common.sh"
SAMPLER2D(u_shadowMap0, 4);

uniform vec4 u_params2;
#define u_depthValuePow u_params2.x

void main()
{
	float depth = unpackRgbaToFloat(texture2D(u_shadowMap0, v_texcoord0) );
	vec3 rgba = pow(vec3_splat(depth), vec3_splat(u_depthValuePow) );
	gl_FragColor = vec4(rgba, 1.0);
}
