#pragma once
#include "Core/Engine.h"

namespace Blueshift {
	namespace Graphics {

		class RenderableComponent : public Scene::Component {
		public:

			void Render(Graphics::RenderSystem* system);
		};

	}
}