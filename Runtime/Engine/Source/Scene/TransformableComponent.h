#pragma once
#include "Core/Math/Math.h"
#include "Scene/Component.h"

namespace Blueshift {
	namespace Scene {

		class SceneGraph;
		class TransformableComponent : public Component {
		protected:
			Core::Math::Vector3f position;
			Core::Math::Quaternionf orientation;
			Core::Math::Matrix4f world_transform;
			bool local_dirty = true;

			friend class SceneGraph;
		public:
			inline virtual void SetPosition(const Core::Math::Vector3f& Position) {
				position.X = Position.X;
				position.Y = Position.Y;
				position.Z = Position.Z;
				local_dirty = true;
			}
			inline virtual const Core::Math::Vector3f& GetPosition() const {
				return position;
			}

			inline virtual void SetOrientation(const Core::Math::Quaternionf& Orientation) {
				orientation.X = Orientation.X;
				orientation.Y = Orientation.Y;
				orientation.Z = Orientation.Z;
				orientation.W = Orientation.W;
				local_dirty = true;
			}
			inline virtual const Core::Math::Quaternionf& GetOrientation() const {
				return orientation;
			}

			inline virtual const Core::Math::Matrix4f& GetWorldTransform() const {
				return world_transform;
			}

			inline bool IsDirty() const {
				return local_dirty;
			}
			void UpdateWorldTransform();
		};

	}
}