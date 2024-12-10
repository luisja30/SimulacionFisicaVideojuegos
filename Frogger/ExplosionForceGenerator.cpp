#include "ExplosionForceGenerator.h"

ExplosionForceGenerator::ExplosionForceGenerator(Vector3 pos, float radius, float k, float tau, string name)
	: ForceGenerator(pos, name), radius_(radius), k_(k), tau_(tau), timePassed_(0) {
}

void ExplosionForceGenerator::updateForce(Actor* a) {
	if (isActive_) {
		Vector3 pPos = a->getPosition();
		// r = distancia de la explosion
		float rad = (pPos - pos_).magnitude();
		//verificar si la particula esta en el radio de explosion
		if (insideLimits(rad)) {
			Vector3 diffDistance = Vector3(pPos.x - pos_.x, pPos.y - pos_.y, pPos.z - pos_.z);
			Vector3 force = k_ / pow(rad, 2) * diffDistance * exp(-timePassed_ / tau_);
			a->addForce(force);
		}
		timePassed_++;
	}
}

bool ExplosionForceGenerator::insideLimits(float r){
	return r < radius_;
}
