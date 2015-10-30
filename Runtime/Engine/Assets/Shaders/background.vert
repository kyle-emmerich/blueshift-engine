$input a_position
$output eye_dir

#include "bgfx_shader.sh"

void main() {
	vec4 pos4 = vec4(a_position.xy, 0.0, 1.0);
	vec4 unprojected = mul(u_invProj, pos4);
	eye_dir = mul((mat3)u_invView, unprojected);

	gl_Position = vec4(a_position.xy, 0.0, 1.0);
}