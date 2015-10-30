#include "Graphics/Shader.h"
#include "bgfx/bgfxplatform.h"
#include "Storage/File.h"

using namespace Blueshift;
using namespace Graphics;

Shader::Shader(std::string Path) {
	Storage::File file(Path);
	size_t length = file.GetLength();

	data = AllocateRenderData(length);
	file.Read(data->data, length);
	file.Close();
}

Shader::Shader(RenderData Data)
	: data(Data) { }

Shader::~Shader() {
	for (auto handle : uniform_handles) {
		if (handle.idx != bgfx::invalidHandle) {
			bgfx::destroyUniform(handle);
		}
	}
	uniform_handles.clear();
	named_uniforms.clear();
	for (auto* uniform : uniforms) {
		delete uniform;
	}
	uniforms.clear();

	if (handle.idx != bgfx::invalidHandle) {
		bgfx::destroyShader(handle);
	}
}

Shader* Shader::AddUniform(std::string Name, Shader::UniformType Type, size_t Size) {
	if (handle.idx != bgfx::invalidHandle) {
		throw 0; //bad
	}
	Uniform* uniform = new Uniform;
	uniform->Handle = bgfx::createUniform(Name.c_str(), static_cast<bgfx::UniformType::Enum>(Type), static_cast<uint16_t>(Size));
	uniform->Owner = this;
	uniform->Name = Name;
	uniform->Type = Type;
	uniform->Size = Size;
	uniform_handles.push_back(uniform->Handle);
	uniforms.push_back(uniform);
	named_uniforms[uniform->Name] = uniform;
	return this;
}

Shader* Shader::Complete() {
	handle = bgfx::createShader(data);
	return this;
}