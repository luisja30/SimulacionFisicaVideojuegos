#include "RigidBody.h"

RigidBody::RigidBody(PxPhysics* gPhysics, PxScene* gScene, PxShape* sp, Vector3 pos, double m, double v, Vector4 c) :
	Actor(pos, Vector3(0), Vector3(0)), gPhysics_(gPhysics), gScene_(gScene), shape_(sp), mass_(m), volume_(v), color_(c) {

	rigidDynamic_ = gPhysics_->createRigidDynamic(tr_);
	rigidDynamic_->setLinearVelocity(Vector3(0, 0, 0));
	rigidDynamic_->setAngularVelocity(Vector3(0, 0, 0));
	rigidDynamic_->attachShape(*shape_);
	rigidDynamic_->setMass(mass_);
	PxRigidBodyExt::updateMassAndInertia(*rigidDynamic_, mass_ / volume_);

	//Me da errores
	//material_ = gPhysics_->createMaterial(0.1f, 0.1f, 0.0f);
	//shape_->setMaterials(&material_, 1);

	gScene_->addActor(*rigidDynamic_);
	rigidActor_ = rigidDynamic_;

	renderItem_ = new RenderItem(shape_, rigidDynamic_, color_);
	setAliveTime(1000);
	dumping_ = (0.998);
}

RigidBody::~RigidBody() {
	//gScene_->removeActor(*rigidActor_);

	if (rigidDynamic_ != nullptr) {
		rigidDynamic_->release();
		rigidDynamic_ = nullptr;
	}
	renderItem_->release();
	renderItem_ = nullptr;
}

bool RigidBody::integrate(double t) {
	//Actualizo posicion del transform
	Vector3 vel = getVel();
	//vel = vel*  pow(dumping_, t);
	tr_.p += vel * t;

	aliveTime_ -= t;

	if (aliveTime_ <= 0)
		return false;

	return true;
}

Vector3 RigidBody::getPosition() {
	return tr_.p;
}

Vector3 RigidBody::getVel() {
	return rigidDynamic_->getLinearVelocity();
}

double RigidBody::getMass() {
	return rigidDynamic_->getMass();
}

void RigidBody::setLinearVelocity(Vector3 vel) {
	rigidDynamic_->setLinearVelocity(vel);
}

void RigidBody::setAngularVelocity(Vector3 aVel) {
	rigidDynamic_->setAngularVelocity(aVel);
}

void RigidBody::setAliveTime(double aliveTime) {
	aliveTime_ = aliveTime;
}

void RigidBody::setMass(double m) {
	rigidDynamic_->setMass(m);
}

void RigidBody::setTensorInertia(Vector3 t) {
	rigidDynamic_->setMassSpaceInertiaTensor(t);
}

void RigidBody::setMaterial(float restitution, float staticFriction, float dynamicFriction) {
	material_->setRestitution(restitution);
	material_->setStaticFriction(staticFriction);
	material_->setDynamicFriction(dynamicFriction);
}

void RigidBody::addForce(const Vector3& f) {
	rigidDynamic_->addForce(f);
}

void RigidBody::disableGravity() {
	rigidDynamic_->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}
