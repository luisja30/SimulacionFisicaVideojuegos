#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Vector3 pos, Vector3 vel, Vector3 dir, double range) : pos_(pos), vel_(vel), dir_(dir), range_(range) {
}

void ParticleSystem::generateParticle() {
	double x = std::uniform_real_distribution<double>(-30, 30)(rd);
	double y = std::uniform_real_distribution<double>(-30, 30)(rd);
	double z = std::uniform_real_distribution<double>(-30, 30)(rd);

	particles_.push_back(new Particle(pos_, Vector3(x, y, z), Vector3(0, 0, 0)));
}

void ParticleSystem::update(double t) {
	generateParticle();
	for (auto it = particles_.begin(); it != particles_.end(); it++) {
		if (*it != nullptr) {
			(*it)->integrate(t);
			//Comprobar rango de recorrido o si la particula sigue viva
			if (!(*it)->isAlive() || ((*it)->getPosition() - pos_).magnitude() > range_) {
				delete *it;
				it = particles_.erase(it);
			}

		}
	}

	//currentFrec_ -= t;

	////Añadir nueva particula cada cierto tiempo
	//if (currentFrec_ <= 0) {
	//	particles_.push_back(new Particle(pos_, vel_, Vector3(0, 0, 0)));
	//	currentFrec_ = frec_;
	//}
}




