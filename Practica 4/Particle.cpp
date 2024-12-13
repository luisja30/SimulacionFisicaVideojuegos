#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel)
	: pose_(pos), vel_(vel), acel_(acel), dumping_(0.998), aliveTime_(100), isAlive_(true), semi_(false){
	//isAlive_ = true;
	//pose_ = PxTransform(pos.x, pos.y, pos.z);
	renderItem_ = new RenderItem(CreateShape(PxSphereGeometry(5.0f)), &pose_, { 0.0, 0.0, 1.0, 1.0 });
}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r)
	: pose_(pos), vel_(vel), acel_(acel), dumping_(0.998), aliveTime_(100), isAlive_(true), semi_(false) {
	renderItem_ = new RenderItem(CreateShape(PxSphereGeometry(r)), &pose_, { 0.0, 0.0, 1.0, 1.0 });
}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime) 
	: pose_(pos), vel_(vel), acel_(acel), dumping_(0.998), aliveTime_(aliveTime), isAlive_(true), semi_(false) {
	renderItem_ = new RenderItem(CreateShape(PxSphereGeometry(r)), &pose_, { 0.0, 0.0, 1.0, 1.0 });
}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime, double mass) 
	: pose_(pos), vel_(vel), acel_(acel), dumping_(0.998), aliveTime_(aliveTime), isAlive_(true), semi_(false) {
	setMass(mass);
	force_ = { 0,0,0 };
	renderItem_ = new RenderItem(CreateShape(PxSphereGeometry(r)), &pose_, { 0.0, 0.0, 1.0, 1.0 });
}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime, double mass, particleForm form)
	: pose_(pos), vel_(vel), acel_(acel), dumping_(0.8), aliveTime_(aliveTime), isAlive_(true), semi_(false) {
	setMass(mass);
	force_ = { 0,0,0 };
	if (form == BOX)
		renderItem_ = new RenderItem(CreateShape(PxBoxGeometry(r, r, r)), &pose_, { 0.0, 0.0, 1.0, 1.0 });
	else if (form == PLANE)
		renderItem_ = new RenderItem(CreateShape(PxBoxGeometry(r * 10, r / 10, r * 10)), &pose_, { 0.0,0.0,1.0,1.0 });
	else
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

	//Calcular aceleracion a partir de la fuerza y masa
	acel_ = force_ / getMass();


	if (!semi_) {
		//euler
		pose_.p += vel_ * t;
		vel_ += acel_ * t;
		vel_ *= pow(dumping_, t);
	}
	else {
		//euler semi-implicito
		vel_ += acel_ * t;
		pose_.p += vel_ * t;
		vel_ *= pow(dumping_, t);
	}

	//Actualizamos el movimiento
	//vel_ += acel_ * t; //aplicando la fuerza de un generador de fuerzas
	//vel_ = vel_ + (acel_ * t);  //aplicando aceleracion
	//vel_ *= pow(dumping_, t);
	//pose_.p += vel_ * t;

	//Decrementamos tiempo de vida y dejamos de actualizar si la particula no está viva
	aliveTime_ -= t;

	if (aliveTime_ <= 0)
		isAlive_ = false;

	//Borrar fuerza
	clearForce();
}

void Particle::setColor(Vector4 color) {
	renderItem_->color = color;
}

void Particle::setMass(double m) {
	mass_ = m;
	massInv_ = 1 / mass_;
}

void Particle::setSemiEuler() {
	semi_ = true;
}

void Particle::addForce(const Vector3& f) {
	force_ += f;
}

void Particle::clearForce() {
	force_ *= 0;
}

bool Particle::isGrounded() {
	return pose_.p.y <= 0;
}

Vector3 Particle::getPosition() const {
	return pose_.p;
}

Vector3 Particle::getVel() const {
	return vel_;
}

double Particle::getMass() const {
	return mass_;
}

double Particle::getInvMass() const {
	return massInv_;
}

bool Particle::isAlive() const {
	return isAlive_;
}
