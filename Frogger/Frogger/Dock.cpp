#include "Dock.h"

Dock::Dock(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, Vector3 anchorPos, double lenght, double k)
	:Enemy(gPhysics, gScene, CreateShape(PxSphereGeometry(20)), pos, 1.0, 1.0, Vector4(1, 1, 0, 1)), 
	initPos_(pos) {

	//Tensor de inercia para la esfera
	float I = (2 / 5) * getMass() * pow(20, 2);
	setTensorInertia(Vector3(I, I, I));

	setMass(1);
	rigidActor_->setName("DockEnemy");
	anchorForce_ = new AnchoredSpringFG(k , lenght, anchorPos);
	forceRegistry_ = new ActorForceRegistry();
	forceRegistry_->addRegistry(anchorForce_, this);

	//para que permanezca levitando en el aire y que no toque el suelo
	disableGravity();
}

Dock::~Dock() {
	delete forceRegistry_;
	forceRegistry_ = nullptr;
	delete anchorForce_;
	anchorForce_ = nullptr;
}

void Dock::resetPosition() {
	tr_.p = initPos_;
	rigidDynamic_->setGlobalPose(tr_);
	rigidDynamic_->setLinearVelocity(Vector3(0));
	rigidDynamic_->setAngularVelocity(Vector3(0));
}

void Dock::updateMove(double t) {
	RigidBody::integrate(t);
	forceRegistry_->updateForces();
}

void Dock::checkCollisions(Vector3 pos) {

}
