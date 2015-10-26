#pragma once
#include "Scene/TransformableComponent.h"
#include "Graphics/Model/IMeshData.h"

namespace Blueshift {
	namespace Scene {

		class MeshComponent : public TransformableComponent {
		protected:
			Graphics::Model::IMeshData* mesh_instance;
		public:
			inline void SetMeshInstance(Graphics::Model::IMeshData* data) {
				this->mesh_instance = data;
			}
			inline Graphics::Model::IMeshData* GetMeshInstance() const { 
				return this->mesh_instance; 
			}
		};

	}
}