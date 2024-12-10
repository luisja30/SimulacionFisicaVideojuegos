#pragma once

#include "ForceGenerator.h"
#include "core.hpp"
using namespace physx;

class BuoyancyForceGenerator :public ForceGenerator {
protected:
	float height_, volume_, liquid_density_;
	float gravity_ = 9.8;

	Particle* liquid_particle;
public:
	BuoyancyForceGenerator(Vector3 pos, float h, float V, float d);

	inline float getVolume() { return volume_; };
	inline void setVolume(float v) { volume_ = v; };
	virtual void updateForce(Actor* a) override;
	virtual ~BuoyancyForceGenerator();

};

