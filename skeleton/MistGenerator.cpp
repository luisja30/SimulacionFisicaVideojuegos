#include "MistGenerator.h"

MistGenerator::MistGenerator(Vector3 pos, double radius) : ParticleGenerator(pos, 1000), radius_(radius) {
}

void MistGenerator::generateParticle() {

	double posX = std::normal_distribution<double>(origin_.x - radius_, origin_.x + radius_)(rd);
	double posY = std::normal_distribution<double>(origin_.y - radius_, origin_.y + radius_)(rd);
	double posZ = std::normal_distribution<double>(origin_.z - radius_, origin_.z + radius_)(rd);
	Vector3 newPos = Vector3(posX, posY, posZ);

	double velX = std::uniform_real_distribution<double>(-1, 1)(rd);
	double velY = std::uniform_real_distribution<double>(-1, 1)(rd);
	double velZ = std::uniform_real_distribution<double>(-1, 1)(rd);
	Vector3 newVel = Vector3(velX, velY, velZ);

	//Damos un tiempo de vida aleatorio
	double newLifeTime = std::uniform_real_distribution<double>(9, 15)(rd);
	//Añadimos particula
	Particle* newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1.0f, newLifeTime);
	newParticle->setColor({ 1.0,1.0,1.0,1.0 });
	particles_.push_back(newParticle);
}

void MistGenerator::update(double t) {
	generateParticle();
	for (auto it = particles_.begin(); it != particles_.end(); it++) {
		if (*it != nullptr) {
			(*it)->integrate(t);
			//Comprobar rango de recorrido o si la particula sigue viva
			if (!(*it)->isAlive() || ((*it)->getPosition() - origin_).magnitude() > range_) {
				delete* it;
				it = particles_.erase(it);
			}
		}
	}
}