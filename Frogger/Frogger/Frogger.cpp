#include "Frogger.h"

Frogger::Frogger(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos)
	: RigidBody(gPhysics, gScene, CreateShape(PxBoxGeometry(10, 10, 10)), pos, 1.0, 1.0, Vector4(0, 1, 0, 1)),
	jumpSpeed_(5) {
}

bool Frogger::integrate(double t) {
	return RigidBody::integrate(t);
}

void Frogger::keyPressed(char key) {
}

void Frogger::move(Vector3 v) {
	//vel_ = 
}
