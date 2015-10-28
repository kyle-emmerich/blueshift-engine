#include "Graphics/Shader.h"
#include "bgfx/bgfxplatform.h"

using namespace Blueshift;
using namespace Graphics;

Shader::Shader(uint8_t* data, size_t size)
	: data(data) {
	handle = bgfx::createShader(bgfx::makeRef(data, static_cast<uint32_t>(size)));

	uint16_t num_uniforms = bgfx::getShaderUniforms(handle);
	if (num_uniforms > 0) {
		uniform_handles.resize(static_cast<size_t>(num_uniforms));
		bgfx::getShaderUniforms(handle, &(uniform_handles[0]), num_uniforms);
	}
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
	if (uniform_handles.size() <= uniforms.size()) {
		throw 0; //TODO exception
	}
	Uniform* uniform = new Uniform;
	uniform->Handle = uniform_handles[uniforms.size()];
	uniform->Owner = this;
	uniform->Name = Name;
	uniform->Type = Type;
	uniform->Size = Size;
	uniform_handles.push_back(uniform->Handle);
	uniforms.push_back(uniform);
	named_uniforms[uniform->Name] = uniform;
	return *this;
}