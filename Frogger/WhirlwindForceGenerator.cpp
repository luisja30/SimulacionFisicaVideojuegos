#include "WhirlwindForceGenerator.h"

WhirlwindForceGenerator::WhirlwindForceGenerator(Vector3 pos, float size, float k, string name)
	: WindForceGenerator(pos, Vector3(0), size, 0, 0, name), k_(k) {
}

void WhirlwindForceGenerator::updateForce(Actor* a) {
	if (insideLimits(a->getPosition()) && isActive_) {
		Vector3 pPos = a->getPosition();
		Vector3 distanceDiff = Vector3(-(pPos.z - pos_.z), 50 - (pPos.y - pos_.y), pPos.x - pos_.x);
		Vector3 force = k_ * distanceDiff;
		a->addForce(force);
	}
}

bool WhirlwindForceGenerator::insideLimits(Vector3 pos) {
	return pos.x < pos_.x + size_
		&& pos.x > pos_.x - size_
		&& pos.z < pos_.z + size_
		&& pos.z > pos_.z - size_;
}
