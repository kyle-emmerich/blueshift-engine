#include "ShaderProgram.h"

using namespace Blueshift;
using namespace Graphics;

ShaderProgram::ShaderProgram(Shader* compute)
	: compute_shader(compute) {
	handle = bgfx::createProgram(compute_shader->handle, false);
	type = Type::Compute;
}

ShaderProgram::ShaderProgram(Shader* vertex, Shader* fragment)
	: vertex_shader(vertex), fragment_shader(fragment) {
	handle = bgfx::createProgram(vertex_shader->handle, fragment_shader->handle, false);
	type = Type::Render;
}

ShaderProgram::~ShaderProgram() {
	if (compute_shader != nullptr) {
		delete compute_shader;
	}
	if (vertex_shader != nullptr) {
		delete vertex_shader;
	}
	if (fragment_shader != nullptr) {
		delete fragment_shader;
	}
	bgfx::destroyProgram(handle);
}