#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator {
protected:
	double k_; //Elastic Coeff
	double resting_length_;
	Particle* other_;
	bool is_elastic_;

public:
	SpringForceGenerator(Vector3 pos, double k, double resting_lenght, Particle* other, bool elastic = false);

	virtual void updateForce(Particle* particle) override;
	inline void setK(double k) { k_ = k; }
	inline double getK(){return k_; }
	virtual ~SpringForceGenerator() {}
};

