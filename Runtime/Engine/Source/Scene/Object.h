#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <string>

#include "Scene/Component.h"
#include "Core/Serialize/Serialize.h"

namespace Blueshift {
	namespace Scene {

		class Object {
		private:
			std::vector<Component::Handle> components;
			Core::Math::Matrix4f transform = Core::Math::IdentityMatrix4f;

			std::string name;
		protected:
#pragma pack(push, 8)
			struct serialized {
				Core::Serialize::SerializedString name;
				Core::Serialize::SerializedList components;

				Core::Math::Matrix4f transform;
			};
#pragma pack(pop)
		public:

			inline const Core::Math::Matrix4f& GetTransform() const {
				return transform;
			}
			inline void SetTransform(const Core::Math::Matrix4f& trans) {
				transform = trans;
			}

			inline const std::vector<Component::Handle>& GetComponents() const {
				return components;
			}
		};

	}
}