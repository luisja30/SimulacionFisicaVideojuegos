#pragma once

#include "WindForceGenerator.h"

class WhirlwindForceGenerator :public WindForceGenerator {
protected:
	float k_;
public:
	WhirlwindForceGenerator(Vector3 pos, float size, float k, string name);
	virtual void updateForce(Actor* a) override;

	bool insideLimits(Vector3 pos);
};

