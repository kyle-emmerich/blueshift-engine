#include "Physics/ColliderComponent.h"
#include "Physics/Interop/Math.h"

using namespace Blueshift;
using namespace Physics;
using namespace Core::Math;

ColliderComponent::~ColliderComponent() {
	if (shape != nullptr) {
		delete shape;
	}
}

void ColliderComponent::BoxShape(Vector3f half_extents) {
	if (shape != nullptr) {
		throw 0; //TODO: exception; already initialized
	}

	shape = new btBoxShape(Interop::ToBullet(half_extents));
	type = ColliderShapeType::Box;
}

void ColliderComponent::SphereShape(float radius) {
	if (shape != nullptr) {
		throw 0; //TODO: exception; already initialized
	}

	shape = new btSphereShape(static_cast<btScalar>(radius));
	type = ColliderShapeType::Box;
}

void ColliderComponent::CapsuleShape(float radius, float height, CardinalAxis axis) {
	if (shape != nullptr) {
		throw 0; //TODO: exception; already initialized
	}

	switch (axis) {
	case CardinalAxis::Up:
	{
		shape = new btCapsuleShape(static_cast<btScalar>(radius), static_cast<btScalar>(height));
	}
	break;
	case CardinalAxis::Forward:
	{
		shape = new btCapsuleShapeZ(static_cast<btScalar>(radius), static_cast<btScalar>(height));
	}
	break;
	case CardinalAxis::Right:
	{
		shape = new btCapsuleShapeX(static_cast<btScalar>(radius), static_cast<btScalar>(height));
	}
	break;
	}
	alignment = axis;
	type = ColliderShapeType::Capsule;
}

void ColliderComponent::CylinderShape(Vector3f half_extents, CardinalAxis axis) {
	if (shape != nullptr) {
		throw 0; //TODO: exception; already initialized
	}

	switch (axis) {
	case CardinalAxis::Up:
	{
		shape = new btCylinderShape(Interop::ToBullet(half_extents));
	}
	break;
	case CardinalAxis::Forward:
	{
		shape = new btCylinderShapeZ(Interop::ToBullet(half_extents));
	}
	break;
	case CardinalAxis::Right:
	{
		shape = new btCylinderShapeX(Interop::ToBullet(half_extents));
	}
	break;
	}
	alignment = axis;
	type = ColliderShapeType::Cylinder;
}

void ColliderComponent::ConeShape(float radius, float height, CardinalAxis axis) {
	if (shape != nullptr) {
		throw 0; //TODO: exception; already initialized
	}

	switch (axis) {
	case CardinalAxis::Up:
	{
		shape = new btConeShape(static_cast<btScalar>(radius), static_cast<btScalar>(height));
	}
	break;
	case CardinalAxis::Forward:
	{
		shape = new btConeShapeZ(static_cast<btScalar>(radius), static_cast<btScalar>(height));
	}
	break;
	case CardinalAxis::Right:
	{
		shape = new btConeShapeX(static_cast<btScalar>(radius), static_cast<btScalar>(height));
	}
	break;
	}
	alignment = axis;
	type = ColliderShapeType::Cone;
}

void ColliderComponent::ConvexHullShape(std::vector<Vector3f>& vertices) {
	if (shape != nullptr) {
		throw 0; //TODO: exception; already initialized
	}
	btConvexHullShape* hull = new btConvexHullShape;
	for (Vector3f v : vertices) {
		hull->addPoint(Interop::ToBullet(v));
	}
	shape = hull;
	type = ColliderShapeType::ConvexHull;
}