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
	Dock(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, Vector3 anchorPos, double lenght, double k);
	virtual ~Dock();
	void resetPosition();

	Vector3 getInitPos() const { return initPos_; };

	virtual void updateMove(double t) override;
	virtual void checkCollisions(Vector3 pos) override;

};

