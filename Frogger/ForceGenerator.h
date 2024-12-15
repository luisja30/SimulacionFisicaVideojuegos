#pragma once
#include "Particle.h"
#include "RigidBody.h"

using namespace std;

class ForceGenerator {
protected:
	Vector3 pos_;
	//double t_;
	string name_;
	bool isActive_;
public:
	ForceGenerator(Vector3 p, string name) : pos_(p), name_(name), isActive_(true) {};
	virtual ~ForceGenerator() {};

	virtual void updateForce(Actor* a) = 0;

	/*void updateTime(double t) {
		t_ += t;
	}*/
	string getName() const {
		return name_;
	}

	void setActive(bool active) {
		isActive_ = active;
	}

	void changeOriginPos(Vector3 pos) {
		pos_ = pos;
	}

	inline bool getActive() { return isActive_; }
};
