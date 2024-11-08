#include "ParticleForceRegistry.h"

ParticleForceRegistry::ParticleForceRegistry() {
}

void ParticleForceRegistry::addRegistry(ForceGenerator* forceGen, Particle* p) {
	//Registramos la particula asociada a un determinado generador de fuerzas
	registryForces_.insert({ forceGen,p });
}

void ParticleForceRegistry::updateForces() {
	//Aplicamos la fuerza correspondiente a la particula
	for (auto it = registryForces_.begin(); it != registryForces_.end(); it++) {
		it->first->updateForce(it->second);
	}
}
