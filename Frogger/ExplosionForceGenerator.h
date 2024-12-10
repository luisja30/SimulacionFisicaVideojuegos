#pragma once

#include "ForceGenerator.h"
#include <cmath>

class ExplosionForceGenerator : public ForceGenerator {
protected:

	// k = intensidad de la explosion
	// tau = constante de tiempo de la explosion
	float radius_, k_, tau_, timePassed_;

public:
	ExplosionForceGenerator(Vector3 pos, float radius, float k, float tau, string name);
	//virtual void updateForce(Particle* p) override;
	//virtual void updateForce(RigidBody* r) override;
	virtual void updateForce(Actor* a) override;
	bool insideLimits(float r);

};

