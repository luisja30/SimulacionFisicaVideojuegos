#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(double k, double resting_lenght, Particle* other)
	: ForceGenerator(Vector3(0, 0, 0), " ") {
	k_ = k;
	resting_length_ = resting_lenght;
	other_ = other;
}

void SpringForceGenerator::updateForce(Particle* particle) {
	Vector3 relative_pos_vector = other_->getPosition() - particle->getPosition();
	Vector3 force;

	const float lenght = relative_pos_vector.normalize();
	const float delta_x = lenght - resting_length_;

	force = relative_pos_vector * delta_x * k_;

	particle->addForce(force);
}
