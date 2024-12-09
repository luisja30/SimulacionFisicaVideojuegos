#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g, string name) : ForceGenerator(Vector3(0), name), gravity_(g) {
}

void GravityForceGenerator::updateForce(Actor* a) {
	a->addForce(gravity_ * a->getMass());
}
