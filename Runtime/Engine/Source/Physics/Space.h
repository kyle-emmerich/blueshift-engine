#pragma once
#include "btBulletDynamicsCommon.h"

namespace Blueshift {
	namespace Physics {

		class Space {
		private:
			struct {
				btCollisionConfiguration* collision_config;
				btCollisionDispatcher* dispatcher;
				btBroadphaseInterface* pair_cache;
				btConstraintSolver* solver;
				btDiscreteDynamicsWorld* world;
			} _internal;

			double fixed_dt = 1.0 / 60.0;
			int max_substeps = 10;
		public:
			 Space();
			~Space();

			void Step(double dt);
		};

	}
}