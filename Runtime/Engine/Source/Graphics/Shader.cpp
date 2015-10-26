#include "Graphics/Shader.h"
#include "bgfx/bgfxplatform.h"

using namespace Blueshift;
using namespace Graphics;

Shader::Shader(uint8_t* data, size_t size) {
	handle = bgfx::createShader(bgfx::makeRef(data, static_cast<uint32_t>(size)));
	
	uint16_t num_uniforms = bgfx::getShaderUniforms(handle);
	uniform_handles.resize(num_uniforms);
	bgfx::getShaderUniforms(handle, &(uniform_handles[0]));
}

Shader::~Shader() {
	bgfx::destroyShader(handle);
	if (data != nullptr) {
		delete[] data;
	}

	for (auto handle : uniform_handles) {
		bgfx::destroyUniform(handle);
	}
	uniform_handles.clear();
	named_uniforms.clear();
	for (auto* uniform : uniforms) {
		delete uniform;
	}
	uniforms.clear();
}

Shader& Shader::AddUniform(std::string Name, Shader::UniformType Type, size_t Size) {
	if (uniforms.size() == uniform_handles.size()) {
		throw 0; //TODO: add exception for too many uniforms
	}
	Uniform* uniform = new Uniform;
	uniform->Handle = uniform_handles[uniforms.size()];
	uniform->Owner = this;
	uniform->Name = Name;
	uniform->Type = Type;
	uniform->Size = Size;
	named_uniforms[uniform->Name] = uniform;
	return *this;
}