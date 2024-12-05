#include "RigidForceRegistry.h"

RigidForceRegistry::RigidForceRegistry() {
}

RigidForceRegistry::~RigidForceRegistry() {
	registryForcesR_.clear();
}

void RigidForceRegistry::addRegistry(ForceGenerator* forceGen, RigidBody* r) {
	registryForcesR_.insert({ forceGen,r });
}

void RigidForceRegistry::deleteRigidRegistry(RigidBody* r) {
	for (auto it = registryForcesR_.begin(); it != registryForcesR_.end();) {
		if (it->second == r)
			it = registryForcesR_.erase(it);
		else it++;
	}
}

void RigidForceRegistry::deleteForceRegistry(ForceGenerator* fg) {
	registryForcesR_.erase(fg);
}

void RigidForceRegistry::updateForces() {
	for (auto it = registryForcesR_.begin(); it != registryForcesR_.end(); it++) {
		it->first->updateForce(it->second);
	}
}
