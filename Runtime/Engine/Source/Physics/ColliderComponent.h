#pragma once
#include "btBulletDynamicsCommon.h"
#include "Scene/TransformableComponent.h"
#include "Scene/SceneGraph.h"
#include "Core/Exceptions.h"

namespace Blueshift {
	namespace Physics {

		class ColliderComponentAlreadyInitializedError : public RuntimeError {
		public:
			ColliderComponentAlreadyInitializedError() : RuntimeError("ColliderComponent already initialized") {}
		};

		enum class ColliderShapeType {
			Invalid = 0,
			Box,
			Sphere,
			Capsule,
			Cylinder,
			Cone,
			ConvexHull,
			TriangleMesh
		};
		class RigidbodyComponent;
		class ColliderComponent : public Scene::TransformableComponent {
		protected:
			btCollisionShape* shape = nullptr;
			ColliderShapeType type = ColliderShapeType::Invalid;
			Core::Math::CardinalAxis alignment = Core::Math::CardinalAxis::Up;

			float mass = 1.0f;

			friend class RigidbodyComponent;
		public:
			ColliderComponent(Scene::SceneGraph* graph, Scene::Object* object) 
				: Scene::TransformableComponent(graph, object) {}
			virtual ~ColliderComponent();

			void BoxShape(Core::Math::Vector4 half_extents);
			void SphereShape(float radius);
			void CapsuleShape(float radius, float height, Core::Math::CardinalAxis axis = Core::Math::CardinalAxis::Up);
			void CylinderShape(Core::Math::Vector4 half_extents, Core::Math::CardinalAxis axis = Core::Math::CardinalAxis::Up);
			void ConeShape(float radius, float height, Core::Math::CardinalAxis axis = Core::Math::CardinalAxis::Up);
			void ConvexHullShape(std::vector<Core::Math::Vector4>& vertices);
			
			inline ColliderShapeType GetType() const {
				return type;
			}

			inline float GetMass() const {
				return mass;
			}
			inline void SetMass(float m) {
				mass = m;
			}

			inline void MakeStatic() {
				mass = 0.0f;
			}
			inline bool IsStatic() const {
				return mass == 0.0f;
			}
		};
	}
}
