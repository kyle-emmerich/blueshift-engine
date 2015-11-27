#pragma once
#include "btBulletDynamicsCommon.h"
#include "Scene/Component.h"
#include "Scene/SceneGraph.h"

namespace Blueshift {
	namespace Physics {

		class RigidbodyComponent : public Scene::Component, public btMotionState {
		protected:
			btRigidBody* body = nullptr;
			btCompoundShape* compound_shape = nullptr;

			float total_mass = 1.0f;
		public:
			RigidbodyComponent(Scene::SceneGraph* scene, Scene::Object* object);
			virtual ~RigidbodyComponent();

			void Initialize();

			//Bullet interface
			virtual void getWorldTransform(btTransform& transform) const;
			virtual void setWorldTransform(const btTransform& transform);
		};

	}
}