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
			Core::Math::Matrix4 transform = Core::Math::Matrix4::Identity;

			std::string name;
		protected:
#pragma pack(push, 8)
			struct serialized {
				Core::Serialize::SerializedString name;
				Core::Serialize::SerializedList components;

				Core::Math::Matrix4 transform;
			};
#pragma pack(pop)
		public:

			inline const Core::Math::Matrix4& GetTransform() const {
				return transform;
			}
			inline void SetTransform(const Core::Math::Matrix4& trans) {
				transform = trans;
			}

			inline const std::vector<Component::Handle>& GetComponents() const {
				return components;
			}
		};

	}
}