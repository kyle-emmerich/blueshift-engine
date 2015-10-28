$input a_position, a_texcoord, a_normal
$output v_worldpos, v_normal

#include "bgfx_shader.sh"

void main() {
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

	v_worldpos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
	v_normal = a_normal;
}