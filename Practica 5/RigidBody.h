#pragma once

#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "RenderUtils.hpp"
#include <iostream>
using namespace physx;


class RigidBody {
protected:
	PxPhysics* gPhysics_;
	PxScene* gScene_;
	PxShape* shape_;
	PxRigidDynamic* rigidDynamic_;

	Vector3 vel_, acel_;
	Vector4 color_;

	physx::PxTransform pose_;
	RenderItem* renderItem_;
	double dumping_; // 0 < d < 1
	double aliveTime_;
	double density_;
	bool isAlive_;
	bool semi_;
	float radius_;
	double volume_;

	//Fuerzas
	double massInv_, mass_;
	Vector3 force_;
public:
	RigidBody(PxPhysics* gPhysics, PxScene* gScene, PxShape* sp ,Vector3 pos, double m, double v, Vector4 c);
	virtual ~RigidBody();
	void update(double t);
	
	Vector3 getPosition();
	inline bool getAlive() const { return isAlive_; };


	void setVelocity(Vector3 vel);
	void setAngularVelocity(Vector3 aVel);
	void setAliveTime(double aliveTime);
};

