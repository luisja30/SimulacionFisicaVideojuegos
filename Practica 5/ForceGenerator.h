#pragma once
#include "Particle.h"

using namespace std;

class ForceGenerator {
protected:
	Vector3 pos_;
	double t_;
	string name_;
public:
	ForceGenerator(Vector3 p, string name) : pos_(p), name_(name), t_(0) {};
	virtual ~ForceGenerator() {};

	virtual void updateForce(Particle* p) = 0;

	void updateTime(double t) {
		t_ += t;
	}
	string getName() const {
		return name_;
	}
};
