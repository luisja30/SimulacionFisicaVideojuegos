#include "Frogger.h"

Frogger::Frogger(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos)
	: RigidBody(gPhysics, gScene, CreateShape(PxBoxGeometry(15, 15, 15)), pos, 1.0, 1.0, Vector4(0, 1, 0, 1)),
	jumpVel_(100) {
}

bool Frogger::integrate(double t) {
	return RigidBody::integrate(t);
}

void Frogger::keyPressed(char key) {
	//Movimiento
	switch (toupper(key)) {
	case 'I':
		move(Vector3(0, 0, -jumpVel_ * getMass()));
		break;
	case 'K':
		move(Vector3(0, 0, jumpVel_* getMass()));
		break;
	case 'J':
		move(Vector3(-jumpVel_* getMass(), 0, 0));
		break;
	case 'L':
		move(Vector3(jumpVel_* getMass(), 0, 0));
		break;
	}
}

void Frogger::move(Vector3 v) {
	rigidDynamic_->addForce(v);
}
