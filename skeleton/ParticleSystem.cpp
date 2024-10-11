#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Vector3 pos, Vector3 vel, Vector3 dir) : pos_(pos), vel_(vel), dir_(dir), range_(200), frec_(3000.0){
}

void ParticleSystem::update(double t) {
	for (auto it = particles_.begin(); it != particles_.end(); it++) {
		if (*it != nullptr) {
			(*it)->integrate(t);
			//Comprobar rango
			//if ((*it)->getPosition() - )
		}
	}
	//if()
	particles_.push_back(new Particle(pos_, vel_, Vector3(0, 0, 0)));
}


