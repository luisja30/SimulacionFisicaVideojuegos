#include "ParticleForceRegistry.h"

ParticleForceRegistry::ParticleForceRegistry() {
}

ParticleForceRegistry::~ParticleForceRegistry() {
	registryForces_.clear();
}

void ParticleForceRegistry::addRegistry(ForceGenerator* forceGen, Particle* p) {
	//Registramos la particula asociada a un determinado generador de fuerzas
	registryForces_.insert({ forceGen,p });
}

void ParticleForceRegistry::deleteParticleRegistry(Particle* p) {
	for (auto it = registryForces_.begin(); it != registryForces_.end();) {
		if (it->second == p)
			it = registryForces_.erase(it);
		else it++;
	}
};

void ParticleForceRegistry::updateForces() {
	//Aplicamos la fuerza correspondiente a la particula
	for (auto it = registryForces_.begin(); it != registryForces_.end(); it++) {
		it->first->updateForce(it->second);
	}
}
