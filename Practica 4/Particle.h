#pragma once

#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "RenderUtils.hpp"
#include <iostream>
using namespace physx;

enum particleForm {
	SPHERE,
	BOX,
	PLANE
};

class Particle {
protected:
	Vector3 vel_, acel_;
	physx::PxTransform pose_;
	RenderItem* renderItem_;
	double dumping_; // 0 < d < 1
	double aliveTime_;
	bool isAlive_;
	bool semi_;
	float radius_;

	//Fuerzas
	double massInv_, mass_;
	Vector3 force_;

public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acel);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime, double mass);
	Particle(Vector3 pos, Vector3 vel, Vector3 acel, float r, double aliveTime, double mass, particleForm form);
	~Particle();
	
	//Setters
	void setColor(Vector4 color);
	void setMass(double m);
	void setSemiEuler();

	//Getters
	Vector3 getPosition() const;
	Vector3 getVel() const;
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

