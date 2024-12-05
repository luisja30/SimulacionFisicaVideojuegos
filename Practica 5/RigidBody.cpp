#include "RigidBody.h"

RigidBody::RigidBody(PxPhysics* gPhysics, PxScene* gScene, PxShape* sp, Vector3 pos, double m, double v, Vector4 c) :
	gPhysics_(gPhysics), gScene_(gScene), shape_(sp), mass_(m), volume_(v), color_(c), aliveTime_(1000) {

	//double r = 3; // Radio de la esfera
	//double inertia = (2.0 / 3.0) * mass_ * r * r;
	//double inertia = (2.0 / 5.0) * mass_ * r * r;
	
	/*double w = 5;
	double h = 10;
	double d = 6;

	double inertia_h = (1 / 12) * mass_ * ((w * w) + (d * d));
	double inertia_w = (1 / 12) * mass_ * ((d * d) + (h * h));
	double inertia_d = (1 / 12) * mass_ * ((w * w) + (d * d));*/


	//radius_ = 3;
	//PxRigidDynamic* new_solid;
	rigidDynamic_ = gPhysics_->createRigidDynamic(PxTransform(pos));
	rigidDynamic_->setLinearVelocity(Vector3(0,0,0));
	rigidDynamic_->setAngularVelocity(Vector3(0,0,0));
	//PxShape* shape_ad = CreateShape();
	rigidDynamic_->attachShape(*shape_);
	rigidDynamic_->setMass(mass_);
	PxRigidBodyExt::updateMassAndInertia(*rigidDynamic_, mass_ / volume_);

	//Para setear momentos de inercia manualmente
	//PxVec3 inertiaTensor(inertia_w, inertia_h, inertia_d);
	//rigidDynamic_->setMassSpaceInertiaTensor(inertiaTensor);

	gScene_->addActor(*rigidDynamic_);

	renderItem_ = new RenderItem(shape_, rigidDynamic_, color_);
}

RigidBody::~RigidBody() {
	rigidDynamic_->release();
	renderItem_->release();
	renderItem_ = nullptr;
}

bool RigidBody::integrate(double t) {
	aliveTime_ -= t;

	if (aliveTime_ <= 0)
		isAlive_ = false;

	return true;
}

Vector3 RigidBody::getPosition() {
	return rigidDynamic_->getGlobalPose().p;
}

bool RigidBody::insideLimits() {
	//return getPosition();
	return false;
}

void RigidBody::setVelocity(Vector3 vel) {
	rigidDynamic_->setLinearVelocity(vel);
}

void RigidBody::setAngularVelocity(Vector3 aVel) {
	rigidDynamic_->setAngularVelocity(aVel);
}

void RigidBody::setAliveTime(double aliveTime) {
	aliveTime_ = aliveTime;
}