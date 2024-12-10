#pragma once
#include <vector>
#include <map>
#include "ForceGenerator.h"

class RigidForceRegistry {
protected:
	multimap<ForceGenerator*, RigidBody*> registryForcesR_;
public:
	RigidForceRegistry();
	virtual ~RigidForceRegistry();
	void addRegistry(ForceGenerator* forceGen, RigidBody* r);
	void deleteRigidRegistry(RigidBody* r);
	void deleteForceRegistry(ForceGenerator* fg);
	void updateForces();
};

