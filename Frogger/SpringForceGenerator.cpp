#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(Vector3 pos, double k, double resting_lenght, Actor* other, bool elastic)
	: ForceGenerator(pos, "SPRING") {
	k_ = k;
	resting_length_ = resting_lenght;
	other_ = other;
	is_elastic_ = elastic;
}

void SpringForceGenerator::updateForce(Actor* a) {
	if (isActive_) {
		Vector3 relative_pos_vector = other_->getPosition() - a->getPosition();

		//Si queremos simular la goma elastica
		if (is_elastic_ && relative_pos_vector.magnitude() < resting_length_)
			return;

		const float lenght = relative_pos_vector.normalize();
		const float delta_x = lenght - resting_length_;

		Vector3 force = relative_pos_vector * delta_x * k_;

		//a->addForce(force);
		RigidBody* rb = static_cast<RigidBody*>(a);
		rb->setLinearVelocity(force);
	}
}
