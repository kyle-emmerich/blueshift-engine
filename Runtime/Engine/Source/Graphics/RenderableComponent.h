#pragma once
#include "Scene/Scene.h"

namespace Blueshift {
	namespace Graphics {

		class RenderableComponent : public Scene::TransformableComponent {
		public:

			virtual void Render(Graphics::RenderSystem* system) = 0;
		};

	}
}