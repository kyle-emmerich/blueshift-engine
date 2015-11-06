#include "Scene/TransformableComponent.h"
#include "Scene/Object.h"
#include "Scene/SceneGraph.h"
#include "Core/Engine.h"

using namespace Blueshift;
using namespace Scene;
using namespace Core::Math;

TransformableComponent::TransformableComponent(SceneGraph* scene, Object* object) 
	: Component(scene, object) {

}

void TransformableComponent::UpdateWorldTransform() {
	if (!local_dirty) {
		return;
	}

	Matrix4f parent = object->GetTransform();
	Matrix4f local = TranslationMatrix(position) * QuaternionToMatrix4(orientation);
	world_transform = local * parent;
	local_dirty = false;
}