#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(Vector3 pos, double k, double resting_lenght, Particle* other, bool elastic)
	: ForceGenerator(pos, "SPRING") {
	k_ = k;
	resting_length_ = resting_lenght;
	other_ = other;
	is_elastic_ = elastic;
}

SpringForceGenerator::SpringForceGenerator(Vector3 pos, double k, double resting_lenght, RigidBody* other, bool elastic)
	: ForceGenerator(pos, "SPRING") {
		k_ = k;
		resting_length_ = resting_lenght;
		otherR_ = other;
		is_elastic_ = elastic;
}

void SpringForceGenerator::updateForce(Particle* particle) {
	Vector3 relative_pos_vector = other_->getPosition() - particle->getPosition();

	//Si queremos simular la goma elastica
	if (is_elastic_ && relative_pos_vector.magnitude() < resting_length_)
		return;
	
	const float lenght = relative_pos_vector.normalize();
	const float delta_x = lenght - resting_length_;

	Vector3 force = relative_pos_vector * delta_x * k_;

	particle->addForce(force);
}

void SpringForceGenerator::updateForce(RigidBody* rigidBody) {
	Vector3 relative_pos_vector = other_->getPosition() - rigidBody->getPosition();

	//Si queremos simular la goma elastica
	if (is_elastic_ && relative_pos_vector.magnitude() < resting_length_)
		return;

	const float lenght = relative_pos_vector.normalize();
	const float delta_x = lenght - resting_length_;

	Vector3 force = relative_pos_vector * delta_x * k_;

	rigidBody->addForce(force);
}
