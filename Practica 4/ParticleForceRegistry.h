#pragma once
#include <vector>
#include <map>
#include "Particle.h"
#include "ForceGenerator.h"

using namespace std;

class ParticleForceRegistry {
protected:
	multimap<ForceGenerator*, Particle*> registryForces_;
public:
	ParticleForceRegistry();
	virtual ~ParticleForceRegistry();
	void addRegistry(ForceGenerator* forceGen, Particle* p);
	void deleteParticleRegistry(Particle* p);
	void deleteForceRegistry(ForceGenerator* fg);
	void updateForces();
};

