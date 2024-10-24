#include "HoseGenerator.h"

HoseGenerator::HoseGenerator(Vector3 pos, double range) :
	ParticleGenerator(pos, range) {

}
void HoseGenerator::generateParticle() {
	//Generamos una posicion aletaria en X y Z
	double posX =origin_.x;
	double posY = origin_.y;
	double posZ = origin_.z;
	Vector3 newPos = Vector3(posX, posY, posZ);

	//Las particulas se mueven solo en el eje Y
	double velX = std::uniform_real_distribution<double>(-10, 10)(rd);
	double velY = std::uniform_real_distribution<double>(-300, -200)(rd);
	double velZ = std::uniform_real_distribution<double>(-10, 10)(rd);
	Vector3 newVel = Vector3(velX, velY, velZ);

	//float size = std::uniform_real_distribution<float>(1.0f, 6.0f)(rd);
	//Añadimos particula
	Particle* newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 5.0f);
	newParticle->setColor({ 0,0,1,1 });
	particles_.push_back(newParticle);
}

void HoseGenerator::update(double t) {
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
