#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
protected:
	Vector3 gravity_;

public:
	GravityForceGenerator(const Vector3& g);
	virtual void updateForce(Particle* p) override;
};

