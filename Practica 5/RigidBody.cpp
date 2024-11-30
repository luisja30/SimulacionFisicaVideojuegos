#include "RigidBody.h"

RigidBody::RigidBody(PxPhysics* gPhysics, PxScene* gScene, PxShape* sp, Vector3 pos, double m, double v, Vector4 c) :
	gPhysics_(gPhysics), gScene_(gScene), shape_(sp), mass_(m), volume_(v), color_(c), aliveTime_(1000) {

	//radius_ = 3;
	//PxRigidDynamic* new_solid;
	rigidDynamic_ = gPhysics_->createRigidDynamic(PxTransform(pos));
	rigidDynamic_->setLinearVelocity(Vector3(0,0,0));
	rigidDynamic_->setAngularVelocity(Vector3(0,0,0));
	//PxShape* shape_ad = CreateShape();
	rigidDynamic_->attachShape(*shape_);
	rigidDynamic_->setMass(mass_);
	PxRigidBodyExt::updateMassAndInertia(*rigidDynamic_, mass_ / volume_);
	gScene_->addActor(*rigidDynamic_);

	//(2 / 3)* (mass_ * pow(radius_, 2));
	renderItem_ = new RenderItem(shape_, rigidDynamic_, color_);
}

RigidBody::~RigidBody() {
	rigidDynamic_->release();
	renderItem_->release();
	renderItem_ = nullptr;
}

void RigidBody::update(double t) {
	if (!isAlive_)
		return;

	aliveTime_ -= t;

	if (aliveTime_ <= 0)
		isAlive_ = false;
}

Vector3 RigidBody::getPosition() {
	return rigidDynamic_->getGlobalPose().p;
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
