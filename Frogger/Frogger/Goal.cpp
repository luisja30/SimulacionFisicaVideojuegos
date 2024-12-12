#include "Goal.h"

Goal::Goal(Vector3 pos)
	: Particle(pos, Vector3(0), Vector3(0), 35.0, 1000, 1.0, SPHERE), 
	isActive_(true) {

	setColor(Vector4(0, 1, 0, 1));
	confettiGenerator_ = new ActorSystem(pos + Vector3(0, heightGen_, 0), 100);
	confettiGenerator_->setActorMode(PARTICLES);
}

Goal::~Goal() {
	delete confettiGenerator_;
	confettiGenerator_ = nullptr;
}

void Goal::update(double t) {
	confettiGenerator_->update(t);
}

void Goal::createConffetiExplosion() {
	confettiGenerator_->setGenMode(FROGGER_EXPLOSION);
	confettiGenerator_->createFroggerExplosion(100);
	setColor(Vector4(0, 0.3, 0, 1));
}

void Goal::setGoal() {
	setActive(false);
	createConffetiExplosion();
}

bool Goal::insideGoal(Vector3 pos) {
	return pos.x < this->getPosition().x + size_
		&& pos.x > this->getPosition().x - size_
		&& pos.z < this->getPosition().z + size_
		&& pos.z > this->getPosition().z - size_;
}
