#include "Physics/RigidbodyComponent.h"
#include "Physics/Interop/Math.h"
#include "Physics/ColliderComponent.h"
#include "Scene/Object.h"


using namespace Blueshift;
using namespace Physics;
using namespace Scene;
using namespace Core::Math;

RigidbodyComponent::RigidbodyComponent(SceneGraph* scene, Object* object)
	: Component(scene, object) {}

RigidbodyComponent::~RigidbodyComponent() {

}

void RigidbodyComponent::Initialize() {
	btCollisionShape* shape;

	std::vector<ColliderComponent*> colliders;
	auto components = object->GetComponents();
	for (int i = 0; i < components.size(); i++) {
		if (components[i]->GetEndType() == typeid(ColliderComponent)) {
			colliders.push_back(components[i]);
		}
	}

	if (colliders.size() == 1) {
		shape = colliders[0]->shape;
		total_mass = colliders[0]->mass;
	} else {
		btCompoundShape* compound = new btCompoundShape(true, static_cast<int>(colliders.size()));
		for (int i = 0; i < colliders.size(); i++) {
			btTransform transform;
			transform.setOrigin(Interop::ToBullet(colliders[i]->position));
			transform.setRotation(Interop::ToBullet(colliders[i]->orientation));
			compound->addChildShape(transform, colliders[i]->shape);
			total_mass += colliders[i]->mass;
		}
		shape = compound_shape = compound;
	}

	body = new btRigidBody(
		static_cast<btScalar>(total_mass),
		this,
		shape
	);
	
}

void RigidbodyComponent::getWorldTransform(btTransform& transform) const {
	const Matrix4& world = this->object->GetTransform();
	transform.setFromOpenGLMatrix(world.data);
}

void RigidbodyComponent::setWorldTransform(const btTransform& transform) {
	Matrix4 trans;
	transform.getOpenGLMatrix(trans.data);
	this->object->SetTransform(trans);
}