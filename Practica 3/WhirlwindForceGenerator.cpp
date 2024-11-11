#include "WhirlwindForceGenerator.h"

WhirlwindForceGenerator::WhirlwindForceGenerator(Vector3 pos, float size, float k, string name)
	: WindForceGenerator(pos, Vector3(0), size, 0, 0, name), k_(k) {
}

void WhirlwindForceGenerator::updateForce(Particle* p) {
	if (insideLimits(p->getPosition())) {
		Vector3 pPos = p->getPosition();
		Vector3 distanceDiff = Vector3(-(pPos.z - pos_.z),
			50 - (pPos.y - pos_.y),
			pPos.x - pos_.x);
		Vector3 vel = k_ * distanceDiff;
		p->addForce(vel);
	}
}

bool WhirlwindForceGenerator::insideLimits(Vector3 pos) {
	return pos.x < pos_.x + size_
		&& pos.x > pos_.x - size_
		&& pos.z < pos_.z + size_
		&& pos.z > pos_.z - size_;
}
