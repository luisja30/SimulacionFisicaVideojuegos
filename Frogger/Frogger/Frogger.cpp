#include "Frogger.h"

Frogger::Frogger(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos)
	: RigidBody(gPhysics, gScene, CreateShape(PxBoxGeometry(15, 15, 15)), pos, 1.0, 1.0, Vector4(0, 1, 0, 1)),
	jumpVel_(300), initPos_(pos) {

	float I = (1 / 12) * 1 * (pow(15, 2) + pow(15, 2));
	setTensorInertia(Vector3(I, I, I));

	explosionParticles_ = new ActorSystem(initPos_, 100);
	explosionParticles_->setActorMode(PARTICLES);
	explosionParticles_->setGenMode(EXPLOSION_A);
}

Frogger::~Frogger() {
	delete explosionParticles_;
	explosionParticles_ = nullptr;
}

bool Frogger::integrate(double t) {

	if (explosionParticles_ != nullptr)
		explosionParticles_->update(t);
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
}
void Frogger::createDeathExplosion() {

	Vector3 explosionPos = this->getPosition() + Vector3(0, 60, 0);

	explosionParticles_->setOriginPos(explosionPos); //Cambiamos pos del generador de particulas
	explosionParticles_->getExplosionForceGenerator()->changeOriginPos(explosionPos); //Cambiamos pos de origen de la explosion
	explosionParticles_->createExplosion(100);
}
;
