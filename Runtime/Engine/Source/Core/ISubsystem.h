#pragma once
#include "Scene/Component.h"
#include <typeindex>

namespace Blueshift {
	namespace Core {

		class ISubsystem {
		public:
			ISubsystem() : Type(typeid(ISubsystem)) {}

			virtual std::type_index GetTypeID() {
				return Type;
			}

			std::type_index Type;
		};
	}
}