#pragma once
#include "Scene/TransformableComponent.h"

namespace Blueshift {
	namespace Graphics {

		enum class CameraProjection {
			Perspective,
			Orthographic
		};

		__declspec(align(16))
		class CameraComponent : public Scene::TransformableComponent {
		protected:
			Core::Math::Matrix4 inverse_transform;
			Core::Math::Matrix4 projection;
		public:
			CameraComponent(Scene::SceneGraph* scene, Scene::Object* object) : TransformableComponent(scene, object) {}

			virtual void UpdateWorldTransform();
			inline const Core::Math::Matrix4& GetViewTransform() const { return inverse_transform; }

			inline void SetProjection(Core::Math::Matrix4 projection) { this->projection = projection; }
			inline Core::Math::Matrix4 GetProjection() const { return projection; }

			static Core::Math::Matrix4 CreatePerspectiveTransform(float AspectRatio, float VerticalFieldOfViewDeg = 70.0, float Near = 0.1, float Far = 1000000.0);
			static Core::Math::Matrix4 CreateOrthographicTransform(float Width, float Height, float Near = -1.0, float Far = 1.0);
		};

	}
}