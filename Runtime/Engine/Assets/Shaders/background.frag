$input eye_dir
#include "bgfx_shader.sh"

SAMPLERCUBE(u_cubemap, 0);

void main() {
	gl_FragColor = pow(textureCube(u_cubemap, eye_dir), 2.0);
}	
