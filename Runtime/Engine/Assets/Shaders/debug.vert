$input a_position, a_texcoord, a_normal
$output v_worldpos, v_normal

#include "bgfx_shader.sh"

void main() {
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

	vec4 vert_pos = mul(u_modelView, vec4(a_position, 1.0));
	v_worldpos = vert_pos.xyz / vert_pos.w;

	mat3 model_view_rot = mat3(u_modelView[0].xyz, u_modelView[1].xyz, u_modelView[2].xyz);
	vec4 normal = mul(u_modelView, vec4(a_normal.xyz, 0.0));
	v_normal = normal.xyz;
}