#include "Car.h"

Car::Car(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, int d, Vector4 c)
	: Enemy(gPhysics, gScene, CreateShape(PxBoxGeometry(20, 20, 20)), pos, 1.0, 1.0, c) {
	smokeParticles_ = new ActorSystem(gPhysics, gScene, pos, 100);
	float I = (1 / 12) * 1 * (pow(20, 2) + pow(20, 2));
	setTensorInertia(Vector3(I, I, I));
}

Car::~Car() {
	delete smokeParticles_;
	smokeParticles_ = nullptr;
}

bool Car::integrate(double t) {
	return RigidBody::integrate(t);
}

void Car::updateMove(double t) {
}

void Car::checkCollisions(Vector3 pos) {
}

