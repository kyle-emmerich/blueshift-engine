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

			std::string name;
		protected:

		public:

		};

	}
}