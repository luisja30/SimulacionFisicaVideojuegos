#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class WindForceGenerator : public ForceGenerator {
protected:
	//Constantes y velocidad
	float k1_, k2_;
	Vector3 vel_;

	//Área de viento
	float size_;
	RenderItem* renderItem_;

	bool drawZone_;
public:
	WindForceGenerator(Vector3 pos, Vector3 vel, float size, float k1, float k2, string name);
	virtual void updateForce(Actor* a) override;

	bool insideLimits(Vector3 pos);
};

