#pragma once

#include "../RigidBody.h"

class Enemy : public RigidBody {
public:
	Enemy(PxPhysics* gPhysics, PxScene* gScene, PxShape* sp, Vector3 pos, double m, double v, Vector4 c);
	virtual ~Enemy() {};

	//Metodos virtuales puros
	virtual void updateMove(double t) = 0;
	virtual void checkCollisions(Vector3 pos) = 0;
};

