#include "Frogger.h"

Frogger::Frogger(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos)
	: RigidBody(gPhysics, gScene, CreateShape(PxBoxGeometry(15, 15, 15)), pos, 1.0, 1.0, Vector4(0, 1, 0, 1)),
	latVel_(100), upVel_(140), canJump_(true), timer_(0), delayTimer_(1), initPos_(pos) {

	//Seteo manual de inercia para cubo solido
	float I = (1 / 12) * 1 * (pow(15, 2) + pow(15, 2));
	setTensorInertia(Vector3(I, I, I));
	
	rigidDynamic_->setLinearDamping(0.998);
	setMass(1);

	explosionParticles_ = new ActorSystem(initPos_, 100);
	explosionParticles_->setActorMode(PARTICLES);
	explosionParticles_->setGenMode(EXPLOSION_A);

	rigidActor_->setName("player");
}

Frogger::~Frogger() {
	delete explosionParticles_;
	explosionParticles_ = nullptr;
}

bool Frogger::integrate(double t) {

	//Actualizamos el sistema de particulas
	if (explosionParticles_ != nullptr)
		explosionParticles_->update(t);

	//Controlamos el delay del salto de la rana para que no se pueda aplicar fuerza infinita
	if (!canJump_) {
		timer_ += t;
		if (timer_ >= delayTimer_) { //si ha pasado el tiempo de delay, la rana puede volver a saltar 
			canJump_ = true;
			timer_ = 0;
		}
	}

	return RigidBody::integrate(t);
}

void Frogger::keyPressed(char key) {
	if (canJump_) {
		//Movimiento
		switch (toupper(key)) {
		case 'I':
			move(Vector3(0, 0, -upVel_));
			break;
		case 'K':
			move(Vector3(0, 0, upVel_));
			break;
		case 'J':
			move(Vector3(-latVel_, 0, 0));
			break;
		case 'L':
			move(Vector3(latVel_, 0, 0));
			break;
		}
	}
}

void Frogger::move(Vector3 v) {
	rigidDynamic_->addForce(v, PxForceMode::eIMPULSE);
	canJump_ = false;
}

void Frogger::resetPosition() {
	tr_.p = initPos_;
	rigidDynamic_->setGlobalPose(tr_);
	rigidDynamic_->setLinearVelocity(Vector3(0));
	rigidDynamic_->setAngularVelocity(Vector3(0));
	//rigidDynamic_->addForce(Vector3(0));
}
void Frogger::createDeathExplosion() {

	Vector3 explosionPos = this->getPosition() + Vector3(0, 60, 0);

	explosionParticles_->setOriginPos(explosionPos); //Cambiamos pos del generador de particulas
	explosionParticles_->getExplosionForceGenerator()->changeOriginPos(explosionPos); //Cambiamos pos de origen de la explosion
	explosionParticles_->createExplosion(100);
}
;
