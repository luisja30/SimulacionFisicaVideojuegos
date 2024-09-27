#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acel) : vel_(vel), acel_(acel), dumping_(0.998) {
	pose_ = PxTransform(pos.x, pos.y, pos.z);
	renderItem_ = new RenderItem(CreateShape(PxSphereGeometry(5.0f)), &pose_, {0.0, 0.0, 0.0, 1.0});
}

Particle::~Particle() {
	DeregisterRenderItem(renderItem_);
}

void Particle::integrate(double t) {
	vel_ = vel_ + (acel_ * t);
	vel_ = vel_ * pow(dumping_, t);
	pose_.p = pose_.p + (vel_ * t);
}

void Particle::setColor(Vector4 color) {
	renderItem_->color = color;
}
