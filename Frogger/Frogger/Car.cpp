#include "Car.h"

Car::Car(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, int d, Vector4 c)
	: Enemy(gPhysics, gScene, CreateShape(PxBoxGeometry(20, 20, 20)), pos, 1.0, 1.0, c) {

	/*smokeParticles_ = new ActorSystem(gPhysics, gScene, pos - Vector3(0,10,0), 100);
	smokeParticles_->setActorMode(PARTICLES);
	smokeParticles_->setGenMode(FROGGER_SMOKE);
	smokeParticles_->setLimit(200);*/

	float I = (1 / 12) * 1 * (pow(20, 2) + pow(20, 2));
	setTensorInertia(Vector3(I, I, I));
}

Car::~Car() {
	/*delete smokeParticles_;
	smokeParticles_ = nullptr;*/
}

void Car::moveCar(Vector3 m){
	//rigidDynamic_->addForce(m * getMass());
	rigidDynamic_->setLinearVelocity(m);
	//smokeParticles_->setOriginPos(smokeParticles_->getOriginPos() + m);
}

void Car::updateMove(double t) {
	RigidBody::integrate(t);
	//smokeParticles_->update(t);

	moveCar(Vector3(-carVel_, 0, 0));
}

void Car::checkCollisions(Vector3 pos) {

}

