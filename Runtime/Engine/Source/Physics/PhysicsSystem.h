#pragma once
#include "Core/Engine.h"

namespace Blueshift {
	namespace Physics {
	
		class PhysicsSystem : public Core::ISubsystem {

		public:
			PhysicsSystem(Core::Engine* engine);
			virtual ~PhysicsSystem();

			virtual void ProcessComponents(const std::vector<Scene::Component>& components);
		};

	}
}