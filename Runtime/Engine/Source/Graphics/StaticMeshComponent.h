#pragma once
#include "Graphics/RenderableComponent.h"
#include "Graphics/Model/IMeshData.h"
#include "Graphics/Texture/Texture.h"
#include <vector>

namespace Blueshift {
	namespace Graphics {

		class StaticMeshComponent : public RenderableComponent {
		protected:
			Model::MeshDataSection* mesh = nullptr;
			ShaderProgram* program = nullptr;

			Texture::Texture* albedo = nullptr;
			Texture::Texture* material = nullptr;
			Texture::Texture* normal = nullptr;
			

			friend class RenderSystem;
		public:
			StaticMeshComponent(Scene::SceneGraph* graph, Scene::Object* object)
				: RenderableComponent(graph, object) {}
			
			inline virtual void SetMesh(Model::MeshDataSection* Mesh) {
				mesh = Mesh;
			}
			inline virtual void SetProgram(ShaderProgram* Program) {
				program = Program;
			}
			inline virtual void SetAlbedoTexture(Texture::Texture* texture) {
				albedo = texture;
			}
			inline virtual void SetMaterialTexture(Texture::Texture* texture) {
				material = texture;
			}
			inline virtual void SetNormalTexture(Texture::Texture* texture) {
				normal = texture;
			}

			virtual void Render(RenderSystem* system);
		};

	}
}