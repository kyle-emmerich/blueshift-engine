$input a_position, a_normal
$output v_worldpos, v_normal

#include "bgfx_shader.sh"

void main() {
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

	v_worldpos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
	v_normal = mul(u_modelView, vec4(a_normal, 1.0)).xyz;
}