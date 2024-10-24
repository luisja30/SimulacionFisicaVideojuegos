#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Vector3 pos, double range)
	: origin_(pos), range_(range) {
}

ParticleGenerator::~ParticleGenerator() {
	//Recorremos la lista y vamos eliminando las partículas para que no quede basura
	for (auto it = particles_.begin(); it != particles_.end(); it++) {
		delete* it;
		it = particles_.erase(it);
	}
}

//void ParticleGenerator::generateParticle() {
//	double x = std::uniform_real_distribution<double>(-10, 10)(rd);
//	double y = std::uniform_real_distribution<double>(-10, 10)(rd);
//	double z = std::uniform_real_distribution<double>(-10, 10)(rd);
//	particles_.push_back(new Particle(pos_, Vector3(x, y, z), Vector3(0, gravity_, 0)));
//}

//void ParticleGenerator::update(double t) {
//	generateParticle();
//	for (auto it = particles_.begin(); it != particles_.end(); it++) {
//		if (*it != nullptr) {
//			(*it)->integrate(t);
//			//Comprobar rango de recorrido o si la particula sigue viva
//			if (!(*it)->isAlive() || ((*it)->getPosition() - pos_).magnitude() > range_) {
//				delete *it;
//				it = particles_.erase(it);
//			}
//		}
//	}
//
//	//currentFrec_ -= t;
//
//	////Añadir nueva particula cada cierto tiempo
//	//if (currentFrec_ <= 0) {
//	//	particles_.push_back(new Particle(pos_, vel_, Vector3(0, 0, 0)));
//	//	currentFrec_ = frec_;
//	//}
//}




