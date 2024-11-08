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

	double massInv_, mass_;
	Vector3 force_;

public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acel);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime);
	~Particle();
	
	//Setters
	void setColor(Vector4 color);
	void setMass(double m);

	//Getters
	Vector3 getPosition() const;
	double getMass() const;

	void addForce(const Vector3& f);
	void clearForce();

	bool isGrounded();
	bool isAlive() const;

	virtual void integrate(double t);
};

