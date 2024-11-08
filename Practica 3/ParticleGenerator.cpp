#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Vector3 pos, double range)
	: origin_(pos), range_(range) {

	//Creamos las fuerzas
	forceReg_ = new ParticleForceRegistry();
	createForceGenerators();
}

ParticleGenerator::~ParticleGenerator() {
	//Recorremos la lista y vamos eliminando las partículas para que no quede basura
	for (Particle* p : particles_)
		delete p;
	particles_.clear();
}

void ParticleGenerator::createForceGenerators() {
	//Fuerza gravitatoria
	forceGen_.push_back(new GravityForceGenerator(Vector3(0, -9.8, 0), "GRAVITY"));

	//Viento 
	
	//Tornado

	//Explosion
}

ForceGenerator* ParticleGenerator::getForceGenerator(string name) {
	auto it = forceGen_.begin();
	bool found = false;
	ForceGenerator* gen = nullptr;

	while (it != forceGen_.end() && !found) {
		if ((*it)->getName() == name) {
			gen = (*it);
			found = true;
		}
		else it++;
	}
	return gen;
}




