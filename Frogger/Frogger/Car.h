#pragma once

#include "Enemy.h"
#include "../ActorSystem.h"

class Car : public Enemy {
protected:
	//Velocidad
	double carVel_;
	//Direccion y rango de desplazamiento
	int dir_,range_;
	//Posicion inicial del coche
	Vector3 initPos_;
	//Generador de humo
	ActorSystem* smokeParticles_;

public:
	Car(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, double vel, int d, int r, Vector4 c);
	virtual ~Car();
	void moveCar(Vector3 m);
	void resetPosition();

	virtual void updateMove(double t) override;
	virtual void checkCollisions(Vector3 pos) override;


};

