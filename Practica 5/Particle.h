#pragma once

#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "Actor.h"
#include <iostream>
using namespace physx;

enum particleForm {
	SPHERE,
	BOX,
	PLANE
};

class Particle : public Actor {
protected:
	//physx::PxTransform pose_;
	//RenderItem* renderItem_;
	double dumping_; // 0 < d < 1
	//double aliveTime_;
	bool isAlive_;
	bool semi_;
	float radius_;

	//Exclusivo de las particulas
	//Vector3 vel_, acel_;
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
	virtual void setMass(double m) override;
	void setSemiEuler();

	//Getters
	Vector3 getPosition() const;
	virtual Vector3 getVel() override;
	virtual double getMass() override;
	double getInvMass() const;

	//Fuerzas
	virtual void addForce(const Vector3& f) override;
	virtual void clearForce() override;

	//Booleanos
	bool isGrounded();
	bool isAlive() const;

	virtual bool integrate(double t) override;
};

