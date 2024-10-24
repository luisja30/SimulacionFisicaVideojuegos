#pragma once

#include "ParticleGenerator.h"
class MistGenerator : public ParticleGenerator {
protected:
	int radius_;
	virtual void generateParticle() override;
public:
	MistGenerator(Vector3 pos, double radius);
	virtual void update(double t) override;
};

