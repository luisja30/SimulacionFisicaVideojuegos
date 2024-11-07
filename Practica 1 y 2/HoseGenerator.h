#pragma once

#include "ParticleGenerator.h"

class HoseGenerator : public ParticleGenerator {
protected:
	virtual void generateParticle() override;
public:
	HoseGenerator(Vector3 pos, double range);
	virtual void update(double t) override;

};

