#pragma once
#include "bgfx/bgfx.h"
#include "Graphics/Shader.h"

namespace Blueshift {
	namespace Graphics {

		class ShaderProgram {
		public:
			enum class Type {
				Compute = 0,
				Render = 1
			};
		protected:
			bgfx::ProgramHandle handle;
			Type type;

			Shader* compute_shader;
			Shader* vertex_shader;
			Shader* fragment_shader;
		public:
			ShaderProgram(Shader* compute);
			ShaderProgram(Shader* vertex, Shader* fragment);
			~ShaderProgram();

			inline Type GetType() {
				return type;
			}
			inline bool IsCompute() { return type == Type::Compute; }
			inline bool IsRender() { return type == Type::Render; }

			inline Shader& GetComputeShader() {
				return *compute_shader;
			}
			inline Shader& GetVertexShader() {
				return *vertex_shader;
			}
			inline Shader& GetFragmentShader() {
				return *fragment_shader;
			}

			inline const bgfx::ProgramHandle& GetHandle() const {
				return handle;
			}

			inline Shader::Uniform& operator[](std::string uniform) {
				//Check all involved shaders
				if (type == Type::Compute) {
					return (*compute_shader)[uniform];
				}
				if (type == Type::Render) {
					//yes, if both shaders share a uniform name, you will
					//have to disambiguate yourself
					if (vertex_shader->HasUniform(uniform) &&
						fragment_shader->HasUniform(uniform)) {
						throw 0; //TODO: ambiguous uniform name exception
					}
					if (vertex_shader->HasUniform(uniform)) {
						return (*vertex_shader)[uniform];
					}
					if (fragment_shader->HasUniform(uniform)) {
						return (*fragment_shader)[uniform];
					}
				}
				throw 0; //TODO: uniform not found exception
			}
		};

	}
}