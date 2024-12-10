#pragma once

#include "Particle.h"

class Projectile : public Particle {
protected:
	Vector3 g_ = { 0, 9.8, 0 };
public:
	Projectile(Vector3 pos, Vector3 vel, Vector3 acel);
	bool integrate(double t) override;
};

