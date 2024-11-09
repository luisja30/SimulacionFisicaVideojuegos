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
	double aliveTime_;
	bool isAlive_;
	float radius_;

	//Fuerzas
	double massInv_, mass_;
	Vector3 force_;

public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acel);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime, double mass);
	~Particle();
	
	//Setters
	void setColor(Vector4 color);
	void setMass(double m);

	//Getters
	Vector3 getPosition() const;
	double getMass() const;
	double getInvMass() const;

	//Fuerzas
	void addForce(const Vector3& f);
	void clearForce();

	//Booleanos
	bool isGrounded();
	bool isAlive() const;

	virtual void integrate(double t);
};

