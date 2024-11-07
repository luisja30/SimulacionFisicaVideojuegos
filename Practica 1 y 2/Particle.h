#pragma once

#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "RenderUtils.hpp"
#include <iostream>


using namespace physx;
class Particle {
protected:
	Vector3 vel_, acel_;
	physx::PxTransform pose_;
	RenderItem* renderItem_;
	double dumping_; // 0 < d < 1
	bool isAlive_;
	double aliveTime_;
	float radius_;
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acel);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime);
	//Particle(Vector3 pos, Vector3 vel, Vector3 acel, double lifeTime);
	~Particle();

	virtual void integrate(double t);
	void setColor(Vector4 color);
	bool isGrounded();
	Vector3 getPosition() const;
	bool isAlive() const;
};

