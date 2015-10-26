#pragma once
#include "Scene/Component.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Matrix.h"

namespace Blueshift {
	namespace Scene {

		class TransformableComponent : Component {
		protected:
			Core::Math::Vector3 scale;
			Core::Math::Vector3 position;
			Core::Math::Quaternion<> orientation;

			Core::Math::Matrix4 transform;
			virtual void recalculate_transform();
		public:

			inline virtual std::type_index GetType() { return typeid(TransformableComponent); }
			inline virtual bool IsTransformable() { return true; }

			inline Core::Math::Vector3 GetPosition() const {
				return position;
			}
			void SetPosition(Core::Math::Vector3 position);

			inline Core::Math::Quaternion<> GetOrientation() const {
				return orientation;
			}
			void SetOrientation(Core::Math::Quaternion<> orientation);

			inline virtual Core::Math::Matrix4 GetTransform() const {
				if (HasParent()) {
					return GetParent()->GetTransform() * transform;
				}
				return Core::Math::IdentityMatrix4;
			}
		};
	}
}