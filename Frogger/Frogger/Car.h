#pragma once

#include "Enemy.h"
#include "../ActorSystem.h"

class Car : public Enemy {
protected:
	double carVel_;
	int dir;
	//Rango de desplazamiento
	int rangeLeft_, rangeRight_;

	//Generador de humo
	ActorSystem* smokeParticles_;

public:
	Car(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, int d, Vector4 c);
	virtual ~Car();
	virtual bool integrate(double t) override;

	virtual void updateMove(double t) override;
	virtual void checkCollisions(Vector3 pos) override;
};

