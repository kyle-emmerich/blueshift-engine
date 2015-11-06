#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <string>

#include "Scene/Component.h"

namespace Blueshift {
	namespace Scene {

		class Object {
		private:
			std::vector<Component*> components;
			Core::Math::Matrix4f transform;

			std::string name;
		protected:

		public:

			inline const Core::Math::Matrix4f& GetTransform() const {
				return transform;
			}
		};

	}
}