#pragma once
#include "Core/Math/Math.h"
#include "Scene/Component.h"

namespace Blueshift {
	namespace Scene {

		class SceneGraph;

		__declspec(align(16))
		class TransformableComponent : public Component {
		protected:
			Core::Math::Vector4 position = Core::Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
			Core::Math::Quaternion orientation = Core::Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
			Core::Math::Matrix4 world_transform = Core::Math::Matrix4::Identity;
			bool local_dirty = true;

			friend class SceneGraph;
		public:
			TransformableComponent(SceneGraph* scene, Object* object);

			inline virtual void SetPosition(const Core::Math::Vector4& Position) {
				position.X = Position.X;
				position.Y = Position.Y;
				position.Z = Position.Z;
				local_dirty = true;
			}
			inline virtual const Core::Math::Vector4& GetPosition() const {
				return position;
			}

			inline virtual void SetOrientation(const Core::Math::Quaternion& Orientation) {
				orientation.X = Orientation.X;
				orientation.Y = Orientation.Y;
				orientation.Z = Orientation.Z;
				orientation.W = Orientation.W;
				local_dirty = true;
			}
			inline virtual const Core::Math::Quaternion& GetOrientation() const {
				return orientation;
			}

			inline virtual const Core::Math::Matrix4& GetWorldTransform() const {
				return world_transform;
			}

			inline bool IsDirty() const {
				return local_dirty;
			}
			void UpdateWorldTransform();
		};

	}
}