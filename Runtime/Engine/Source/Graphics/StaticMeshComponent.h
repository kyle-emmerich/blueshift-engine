#pragma once
#include "Graphics/RenderableComponent.h"
#include "Graphics/Model/IMeshData.h"
#include "Graphics/Texture.h"
#include <vector>

namespace Blueshift {
	namespace Graphics {

		class StaticMeshComponent : public RenderableComponent {
		protected:
			Model::MeshDataSection* mesh;
			ShaderProgram* program;

			Texture* albedo;
			Texture* material;
			Texture* normal;
			

			friend class RenderSystem;
		public:
			
			inline virtual void SetMesh(Model::MeshDataSection* Mesh) {
				mesh = Mesh;
			}
			inline virtual void SetProgram(ShaderProgram* Program) {
				program = Program;
			}
			inline virtual void SetAlbedoTexture(Texture* texture) {
				albedo = texture;
			}
			inline virtual void SetMaterialTexture(Texture* texture) {
				material = texture;
			}
			inline virtual void SetNormalTexture(Texture* texture) {
				normal = texture;
			}

			virtual void Render(RenderSystem* system);
		};

	}
}