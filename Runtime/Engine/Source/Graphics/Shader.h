#pragma once
#include "bgfx/bgfx.h"
#include "Graphics/RenderData.h"
#include <vector>
#include <unordered_map>

namespace Blueshift {
	namespace Graphics {

		class ShaderProgram;
		class Shader {
		public:
			friend class ShaderProgram;
			enum class UniformType {
				Texture = 1,
				Vector = 2,
				Matrix3 = 3,
				Matrix4 = 4,
			};
			struct Uniform {
				bgfx::UniformHandle Handle;
				Shader* Owner;
				std::string Name;
				UniformType Type;
				size_t Size = 1;

				template<typename T>
				inline void operator=(T* data) {
					//not sure if this is even going to work
					if (sizeof(data) != sizeof(T) * Size) {
						//this might not work as expected? idk, might give me size of
						//T*, or the size of a pointer itself.
						//throw 0; //TODO: make exception for wrong size data assignment
					}
					bgfx::setUniform(Handle, reinterpret_cast<const void*>(data), Size);
				}

				template<typename T>
				inline void operator=(std::vector<T> data) {
					if (data.size() != Size) {
						throw 0; //TODO: make exception for wrong size data
					}
					bgfx::setUniform(Handle, reinterpret_cast<const void*>(&data[0]), Size);
				}

				template<typename T>
				inline void operator=(T& data) {
					if (Size != 1) {
						throw 0; //TODO: make exception for bad assignment
					}
					bgfx::setUniform(Handle, reinterpret_cast<const void*>(&data), 1);
				}

				inline void Set(const void* data) {
					bgfx::setUniform(Handle, data, static_cast<uint16_t>(Size));
				}
			};
		protected:
			bgfx::ShaderHandle handle = BGFX_INVALID_HANDLE;
			RenderData data;

			std::vector<bgfx::UniformHandle> uniform_handles;
			std::vector<Uniform*> uniforms;
			std::unordered_map<std::string, Uniform*> named_uniforms;
		public:
			Shader(std::string Path);
			Shader(RenderData Data);
			~Shader();

			Shader* AddUniform(std::string Name, UniformType Type, size_t Size = 1);
			Shader* Complete();

			inline const Uniform* GetUniformInfo(size_t id) const {
				if (id < uniforms.size()) {
					return uniforms[id];
				}
				return nullptr;
			}
			inline const Uniform* GetUniformInfo(std::string name) const {
				if (named_uniforms.find(name) == named_uniforms.end()) {
					return nullptr;
				}
				return named_uniforms.at(name);
			}
			inline Uniform* GetUniformInfo(std::string name) {
				if (named_uniforms.find(name) == named_uniforms.end()) {
					return nullptr;
				}
				return named_uniforms.at(name);
			}
			inline bool HasUniform(std::string name) {
				return named_uniforms.find(name) != named_uniforms.end();
			}

			inline Uniform& operator[](std::string name) {
				if (named_uniforms.find(name) == named_uniforms.end()) {
					throw 0; //TODO: make exception for uniform not found
				}
				return *(named_uniforms.at(name));
			}
		};

	}
}