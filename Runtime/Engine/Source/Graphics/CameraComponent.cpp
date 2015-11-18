#include "Graphics/CameraComponent.h"
#include "Core/Math/Misc.h"
#include <cmath>

using namespace Blueshift;
using namespace Scene;
using namespace Graphics;

void CameraComponent::UpdateWorldTransform() {
	TransformableComponent::UpdateWorldTransform();
	inverse_transform = Core::Math::Inverse(world_transform);
}

Core::Math::Matrix4f CameraComponent::CreatePerspectiveTransform(float AspectRatio, float VerticalFieldOfViewDeg, float Near, float Far) {
	//https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml

	float height = 1.0f / tan(Core::Math::ToRadians(VerticalFieldOfViewDeg) * 0.5f);
	float width = height * (1.0f / AspectRatio);

	float aa = (Far + Near) / (Near - Far);
	float bb = (2.0f * Far * Near) / (Near - Far);

	return Core::Math::Matrix4f {
		width,	0.0f,	0.0f,	0.0f,
		0.0f,	height, 0.0f,	0.0f,
		0.0f,	0.0f,  -aa,		1.0f,
		0.0f,	0.0f,	bb,		0.0f
	};
}

Core::Math::Matrix4f CameraComponent::CreateOrthographicTransform(float Width, float Height, float Near, float Far) {
	//https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
	//Left is assumed to be 0, and Width is the same as right. Same goes for Height.
	return Core::Math::Matrix4f {
		2.0f / Width,	0.0f,			 0.0f,					-1.0f,
		0.0f,			2.0f / Height,	 0.0f,					-1.0f,
		0.0f,			0.0f,			-2.0f / (Far - Near),	-(Far + Near) / (Far - Near),
		0.0f,			0.0f,			 0.0f,					 1.0f
	};
}