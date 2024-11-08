#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel)
	: pose_(pos), vel_(vel), acel_(acel), dumping_(0.998), aliveTime_(100), isAlive_(true) {
	//isAlive_ = true;
	//pose_ = PxTransform(pos.x, pos.y, pos.z);
	renderItem_ = new RenderItem(CreateShape(PxSphereGeometry(5.0f)), &pose_, { 0.0, 0.0, 1.0, 1.0 });
}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r)
	: pose_(pos), vel_(vel), acel_(acel), dumping_(0.998), aliveTime_(100), isAlive_(true) {
	renderItem_ = new RenderItem(CreateShape(PxSphereGeometry(r)), &pose_, { 0.0, 0.0, 1.0, 1.0 });
}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime) 
	: pose_(pos), vel_(vel), acel_(acel), dumping_(0.998), aliveTime_(aliveTime), isAlive_(true) {
	renderItem_ = new RenderItem(CreateShape(PxSphereGeometry(r)), &pose_, { 0.0, 0.0, 1.0, 1.0 });
}

Particle::~Particle() {
	renderItem_->release();
	renderItem_ = nullptr;
}

void Particle::integrate(double t) {
	//Si no esta viva la particula, no devolvemos nada
	if (!isAlive_)
		return;

	//Actualizamos el movimiento
	vel_ = vel_ + (force_ * t);
	vel_ = vel_ * pow(dumping_, t);
	pose_.p = pose_.p + (vel_ * t);

	//Decrementamos tiempo de vida y dejamos de actualizar si la particula no está viva
	aliveTime_ -= t;

	if (aliveTime_ <= 0)
		isAlive_ = false;
}

void Particle::setColor(Vector4 color) {
	renderItem_->color = color;
}

void Particle::setMass(double m) {
	mass_ = m;
}

void Particle::addForce(const Vector3& f) {
	force_ += f;
}

void Particle::clearForce() {
	force_ = { 0,0,0 };
}

bool Particle::isGrounded() {
	return pose_.p.y <= 0;
}

Vector3 Particle::getPosition() const {
	return pose_.p;
}

double Particle::getMass() const {
	return mass_;
}

bool Particle::isAlive() const {
	return isAlive_;
}
