#pragma once
#include <typeindex>

namespace Blueshift {
	namespace Core {

		class ISubsystem {
		public:
			ISubsystem() : Type(typeid(ISubsystem)) {}
			virtual ~ISubsystem() {}

			virtual std::type_index GetTypeID() {
				return Type;
			}

			std::type_index Type;
		};
	}
}