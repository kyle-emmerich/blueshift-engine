#include "Physics/Space.h"

using namespace Blueshift;
using namespace Physics;

Space::Space() {
	//First job is to set up Bullet
	//Make collision configuration
	_internal.collision_config = new btDefaultCollisionConfiguration;
	//Then collision dispatcher; this should be changed to multithreaded version later
	_internal.dispatcher = new btCollisionDispatcher(_internal.collision_config);
	//Now the pair cache
	_internal.pair_cache = new btDbvtBroadphase;
	//Now the solver
	_internal.solver = new btSequentialImpulseConstraintSolver;
	//Finally, the world
	_internal.world = new btDiscreteDynamicsWorld(_internal.dispatcher, _internal.pair_cache, _internal.solver, _internal.collision_config);
	//For debug, let's set gravity.
	_internal.world->setGravity(btVector3(0, -9.81, 0));
}

Space::~Space() {
	delete _internal.world;
	delete _internal.solver;
	delete _internal.pair_cache;
	delete _internal.dispatcher;
	delete _internal.collision_config;
}

void Space::Step(double dt) {
	_internal.world->stepSimulation(dt, max_substeps, fixed_dt);
}