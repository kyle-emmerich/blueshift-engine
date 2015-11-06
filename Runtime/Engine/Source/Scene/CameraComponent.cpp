#include "Scene/CameraComponent.h"
#include "Core/Math/Misc.h"
#include <cmath>

using namespace Blueshift;
using namespace Scene;

void CameraComponent::recalculate_transform() {
	//Let the TransformableComponent calculate itself first
	//TransformableComponent::recalculate_transform();
	//Now find the inverse
	//inverse_transform = Core::Math::Inverse(transform);
}

Core::Math::Matrix4 CameraComponent::CreatePerspectiveTransform(double AspectRatio, double VerticalFieldOfViewDeg, double Near, double Far) {
	//https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml

	double height = 1.0 / tan(VerticalFieldOfViewDeg * Core::Math::DegToRad * 0.5);
	double width = height * (1.0 / AspectRatio);

	double aa = (Far + Near) / (Far - Near);
	double bb = -(2.0 * Far * Near) / (Far - Near);

	return Core::Math::Matrix4 {
		width, 0.0, 0.0, 0.0,
		0.0, height, 0.0, 0.0,
		0.0, 0.0, aa, 1.0,
		0.0, 0.0, bb, 0.0
	};
}

Core::Math::Matrix4 CameraComponent::CreateOrthographicTransform(double Width, double Height, double Near, double Far) {
	//https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
	//Left is assumed to be 0, and Width is the same as right. Same goes for Height.
	return Core::Math::Matrix4 {
		2.0 / Width, 0.0, 0.0, -1.0,
		0.0, 2 / Height, 0.0, -1.0,
		0.0, 0.0, -2.0 / (Far - Near), -(Far + Near) / (Far - Near),
		0.0, 0.0, 0.0, 1.0
	};
}