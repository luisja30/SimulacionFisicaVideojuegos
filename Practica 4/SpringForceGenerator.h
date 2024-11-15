#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator {
protected:
	double k_; //Elastic Coeff
	double resting_length_;
	Particle* other_;

public:
	SpringForceGenerator(double k, double resting_lenght, Particle* other);

	virtual void updateForce(Particle* particle) override;
	inline void setK(double k) { k_ = k; }
	virtual ~SpringForceGenerator() {}
};

