#pragma once
#include "ParticleGenerator.h"

class RainGenerator : public ParticleGenerator {
protected:
	int radius_;
	virtual void generateParticle() override;
public:
	RainGenerator(Vector3 pos, double radius);
	virtual void update(double t) override;
};

