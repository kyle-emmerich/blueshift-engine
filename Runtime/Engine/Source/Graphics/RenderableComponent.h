#pragma once
#include "Scene/Scene.h"

namespace Blueshift {
	namespace Graphics {

		class RenderableComponent : public Scene::TransformableComponent {
		public:
			RenderableComponent(Scene::SceneGraph* graph, Scene::Object* object)
				: TransformableComponent(graph, object) {}

			virtual void Render(Graphics::RenderSystem* system) = 0;
		};

	}
}