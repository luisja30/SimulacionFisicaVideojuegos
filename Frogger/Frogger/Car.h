#pragma once

#include "../RigidBody.h"
#include "../ActorSystem.h"

class Car : public RigidBody {
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
	

};

