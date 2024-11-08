#pragma once
#include "Particle.h"

using namespace std;

class ForceGenerator {
protected:
	Vector3 pos_;
	double t_;
public:
	ForceGenerator(Vector3 p) : pos_(p), t_(0) {};
	virtual ~ForceGenerator() {};

	virtual void updateForce(Particle* p) = 0;

	void updateTime(double t) {
		t_ += t;
	}
};
