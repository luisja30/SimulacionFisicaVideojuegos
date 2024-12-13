#pragma once

#include "Enemy.h"
#include "../ActorForceRegistry.h"
#include "../AnchoredSpringFG.h"

class Dock : public Enemy {
protected:
	ActorForceRegistry* forceRegistry_;
	AnchoredSpringFG* anchorForce_;
	Vector3 initPos_;

public:
	Dock(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, Vector3 anchorPos, double lenght);
	virtual ~Dock();
	void resetPosition();

	virtual void updateMove(double t) override;
	virtual void checkCollisions(Vector3 pos) override;
};

