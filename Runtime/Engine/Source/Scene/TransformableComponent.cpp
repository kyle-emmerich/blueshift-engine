#include "Scene/TransformableComponent.h"

using namespace Blueshift;
using namespace Scene;

void TransformableComponent::recalculate_transform() {
	transform = Core::Math::ScaleMatrix<>(scale);
	transform = transform * Core::Math::QuaternionToMatrix4<>(orientation);
	transform = transform * Core::Math::TranslationMatrix<>(position);
}

void TransformableComponent::SetPosition(Core::Math::Vector3 position) {
	this->position = position; recalculate_transform();
}

void TransformableComponent::SetOrientation(Core::Math::Quaternion<> orientation) {
	this->orientation = orientation; recalculate_transform();
}