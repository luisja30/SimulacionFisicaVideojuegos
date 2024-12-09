#pragma once

#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "RenderUtils.hpp"
#include <iostream>
using namespace physx;

class Actor {
protected:
	physx::PxTransform tr_;
	RenderItem* renderItem_;
	double aliveTime_;

	Vector3 vel_, acel_;

public:
	Actor(Vector3 pos, Vector3 vel, Vector3 acel) : tr_(pos), vel_(vel), acel_(acel) {};

	virtual ~Actor() {};

	virtual bool integrate(double t) = 0;
	virtual void addForce(const Vector3& f) = 0;
	virtual void clearForce() = 0;

	//Getters
	Vector3 getPosition() { return tr_.p; };
	virtual Vector3 getVel() = 0;
	virtual double getMass() = 0;

	//Setters
	void setColor(Vector4 c) { renderItem_->color = c; };
	void setAliveTime(double a) { aliveTime_ = a; };
	virtual void setMass(double m) = 0;
};



