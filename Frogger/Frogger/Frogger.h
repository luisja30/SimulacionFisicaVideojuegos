#pragma once

#include "../RigidBody.h"
#include "../ActorSystem.h"

class Frogger : public RigidBody {
protected:
	double jumpVel_;
	Vector3 initPos_;

	ActorSystem* particles_;

public:
	Frogger(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos);
	virtual ~Frogger() {};
	virtual bool integrate(double t) override;
	void keyPressed(char key);
	void move(Vector3 v);
	void resetPosition();
};

