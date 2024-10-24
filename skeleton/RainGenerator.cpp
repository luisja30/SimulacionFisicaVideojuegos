#include "RainGenerator.h"

RainGenerator::RainGenerator(Vector3 pos, double radius) :
	ParticleGenerator(pos, 1000), radius_(radius) {
}

void RainGenerator::generateParticle() {

	//Generamos una posicion aletaria en X y Z
	double posX = std::normal_distribution<double>(origin_.x - radius_, origin_.x + radius_)(rd);
	double posY = origin_.y;
	double posZ = std::normal_distribution<double>(origin_.z - radius_, origin_.z + radius_)(rd);
	Vector3 newPos = Vector3(posX, posY, posZ);

	//Las particulas se mueven solo en el eje Y
	double velY = -300;
	Vector3 newVel = Vector3(0, velY, 0);

	//Añadimos particula
	Particle* newParticle = new Particle(newPos, newVel, Vector3(0, -10, 0), 1);
	newParticle->setColor({ 0,0,1,1 });
	particles_.push_back(newParticle);
}

void RainGenerator::update(double t) {
    generateParticle();
	for (auto it = particles_.begin(); it != particles_.end(); it++) {
		if (*it != nullptr) {
			(*it)->integrate(t);
			//Comprobar rango de recorrido o si la particula sigue viva
			if (!(*it)->isAlive() || ((*it)->getPosition() - origin_).magnitude() > range_) {
				delete *it;
				it = particles_.erase(it);
			}
		}
	}
}





