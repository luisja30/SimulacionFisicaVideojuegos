#pragma once

#include <PxPhysicsAPI.h>

#include "core.hpp"
#include "RenderUtils.hpp"
#include <iostream>


using namespace physx;
class Particle {
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acel);
	~Particle();

	void integrate(double t);
	void setColor(Vector4 color);
private:
	Vector3 vel_, acel_;
	physx::PxTransform pose_;
	RenderItem* renderItem_;
	double dumping_; // 0 < d < 1
};

