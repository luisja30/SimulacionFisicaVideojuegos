#include "Car.h"

Car::Car(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos,double vel, int d, int r, Vector4 c)
	: Enemy(gPhysics, gScene, CreateShape(PxBoxGeometry(20, 20, 20)), pos, 1.0, 1.0, c),
	dir_(d), range_(r), initPos_(pos), carVel_(vel) {

	setMass(20);
	smokeParticles_ = new ActorSystem(pos + Vector3(0,50,0), 100);
	smokeParticles_->setActorMode(PARTICLES);
	smokeParticles_->setGenMode(FROGGER_SMOKE);
	smokeParticles_->setLimit(200);

	float I = (1 / 12) * 1 * (pow(20, 2) + pow(20, 2));
	setTensorInertia(Vector3(I, I, I));
}

Car::~Car() {
	delete smokeParticles_;
	smokeParticles_ = nullptr;
}

void Car::moveCar(Vector3 m){
	//Movemos el coche
	rigidDynamic_->setLinearVelocity(m);
	//Cambiamos posicion del generador de humo para que se mueva junto al coche
	Vector3 carPosition = this->getPosition();
	smokeParticles_->setOriginPos(carPosition);
	smokeParticles_->getWindForceGenerator()->changeOriginPos(carPosition);

	//Si el coche supera su rango de recorrido, vuelve a su posicion original
	if ((this->getPosition() - initPos_).magnitude() > range_) {
		resetPosition();
	}

}

void Car::resetPosition() {
	tr_.p = initPos_;
	rigidDynamic_->setGlobalPose(tr_);
	rigidDynamic_->setLinearVelocity(Vector3(0));
	rigidDynamic_->setAngularVelocity(Vector3(0));
}

void Car::updateMove(double t) {
	if(smokeParticles_ != nullptr)
		smokeParticles_->update(t);

	RigidBody::integrate(t);

	moveCar(Vector3(carVel_ * dir_, 0, 0));
}

void Car::checkCollisions(Vector3 pos) {

}

