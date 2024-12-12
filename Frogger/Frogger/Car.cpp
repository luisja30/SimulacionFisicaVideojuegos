#include "Car.h"

Car::Car(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, int d, Vector4 c)
	: RigidBody(gPhysics, gScene, CreateShape(PxBoxGeometry(20, 20, 20)), pos, 1.0, 1.0, c) {
	smokeParticles_ = new ActorSystem(gPhysics, gScene, pos, 100);
}

Car::~Car() {
	delete smokeParticles_;
	smokeParticles_ = nullptr;
}

bool Car::integrate(double t) {
	return RigidBody::integrate(t);
}

