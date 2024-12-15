#include "Frogger.h"

Frogger::Frogger(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos)
	: RigidBody(gPhysics, gScene, CreateShape(PxBoxGeometry(15, 15, 15)), pos, 1.0, 1.0, Vector4(0, 1, 0, 1)),
	jumpVel_(300), initPos_(pos) {

	float I = (1 / 12) * 1 * (pow(15, 2) + pow(15, 2));
	setTensorInertia(Vector3(I, I, I));
}

bool Frogger::integrate(double t) {
	return RigidBody::integrate(t);
}

void Frogger::keyPressed(char key) {
	//Movimiento
	switch (toupper(key)) {
	case 'I':
		move(Vector3(0, 0, -jumpVel_));
		break;
	case 'K':
		move(Vector3(0, 0, jumpVel_));
		break;
	case 'J':
		move(Vector3(-jumpVel_, 0, 0));
		break;
	case 'L':
		move(Vector3(jumpVel_, 0, 0));
		break;
	}
}

void Frogger::move(Vector3 v) {
	rigidDynamic_->setLinearVelocity(v);
}

void Frogger::resetPosition() {
	tr_.p = initPos_;
	rigidDynamic_->setGlobalPose(tr_);
	rigidDynamic_->setLinearVelocity(Vector3(0));
	rigidDynamic_->setAngularVelocity(Vector3(0));
};
