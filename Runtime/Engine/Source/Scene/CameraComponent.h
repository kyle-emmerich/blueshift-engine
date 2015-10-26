#pragma once
#include "Scene/TransformableComponent.h"

namespace Blueshift {
	namespace Scene {

		enum class CameraProjection {
			Perspective,
			Orthographic
		};

		class CameraComponent : public TransformableComponent {
		protected:
			Core::Math::Matrix4 inverse_transform;
			Core::Math::Matrix4 projection;

			virtual void recalculate_transform();
		public:
			inline const Core::Math::Matrix4& GetViewTransform() const { return inverse_transform; }

			inline void SetProjection(Core::Math::Matrix4 projection) { this->projection = projection; }
			inline Core::Math::Matrix4 GetProjection() const { return projection; }

			static Core::Math::Matrix4 CreatePerspectiveTransform(double AspectRatio, double VerticalFieldOfViewDeg = 70.0, double Near = 0.01, double Far = 1000000.0);
			static Core::Math::Matrix4 CreateOrthographicTransform(double Width, double Height, double Near = -1.0, double Far = 1.0);
		};

	}
}