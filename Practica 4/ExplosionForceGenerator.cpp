#include "ExplosionForceGenerator.h"

ExplosionForceGenerator::ExplosionForceGenerator(Vector3 pos, float radius, float k, float tau, string name)
	: ForceGenerator(pos, name), radius_(radius), k_(k), tau_(tau), timePassed_(0) {
}

void ExplosionForceGenerator::updateForce(Particle* p) {
	Vector3 pPos = p->getPosition();
	// r = distancia de la explosion
	float r = (pPos - pos_).magnitude();
	//verificar si la particula esta en el radio de explosion
	if (insideLimits(r)) {
		Vector3 diffDistance = Vector3(pPos.x - pos_.x, pPos.y - pos_.y, pPos.z - pos_.z);
		Vector3 force = k_ / pow(r, 2) * diffDistance * exp(-timePassed_ / tau_);
		p->addForce(force);
	}
	timePassed_++;
}

bool ExplosionForceGenerator::insideLimits(float r){
	return r < radius_;
}
