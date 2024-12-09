#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator {
protected:
	double k_; //Elastic Coeff
	double resting_length_;
	Actor* other_;
	bool is_elastic_;

public:
	SpringForceGenerator(Vector3 pos, double k, double resting_lenght, Actor* other, bool elastic = false);

	virtual void updateForce(Actor* a) override;
	inline void setK(double k) { k_ = k; }
	inline double getK(){return k_; }
	virtual ~SpringForceGenerator() {}
};

