#pragma once
#include "Scene/Component.h"

namespace Blueshift {
	namespace Core {

		class ISubsystem {
		public:
			ISubsystem() : Type(typeid(ISubsystem)) {}

			virtual void ProcessComponents(std::vector<Scene::Component>& components) = 0;

			std::type_index Type;
		};
	}
}