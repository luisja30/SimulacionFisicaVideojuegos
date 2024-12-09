#include "WindForceGenerator.h"

WindForceGenerator::WindForceGenerator(Vector3 pos, Vector3 vel, float size, float k1, float k2, string name)
	:ForceGenerator(pos, name), vel_(vel), size_(size), k1_(k1), k2_(k2), drawZone_(false) {
	if (drawZone_) {
		//Renderizamos una caja sobre la que ver la zona de accion de la fuerza
		PxTransform pose(pos.x, pos.y, pos.z);
		renderItem_ = new RenderItem(CreateShape(PxBoxGeometry(size, 1, size)), &pose, { 0.0, 1.0, 1.0, 0.0 });
	}
}

void WindForceGenerator::updateForce(Actor* a) {
	if (insideLimits(a->getPosition()) && isActive_) {
		Vector3 diffVel = vel_ - a->getVel();
		Vector3 windForce = k1_ * diffVel + (k2_ * diffVel.magnitude()) * diffVel;
		a->addForce(windForce);
	}
}

bool WindForceGenerator::insideLimits(Vector3 pos) {
	return pos.x < pos_.x + size_ 
		&& pos.x > pos_.x - size_ 
		&& pos.y < pos_.y + size_ * 2
		&& pos.y > pos_.y - size_ 
		&& pos.z < pos_.z + size_ 
		&& pos.z > pos_.z - size_;
}
