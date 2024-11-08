#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g) : ForceGenerator(Vector3(0)), gravity_(g) {
}

void GravityForceGenerator::updateForce(Particle* p) {
	p->addForce(gravity_ * p->getMass());
}
