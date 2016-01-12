#pragma once
#include "Graphics/RenderableComponent.h"
#include "Graphics/Model/IMeshData.h"
#include "Graphics/Texture.h"
#include <vector>

namespace Blueshift {
	namespace Graphics {

		class StaticMeshComponent : public RenderableComponent {
		protected:
			Model::MeshDataSection* mesh = nullptr;
			ShaderProgram* program = nullptr;

			Texture* albedo = nullptr;
			Texture* material = nullptr;
			Texture* normal = nullptr;
			

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