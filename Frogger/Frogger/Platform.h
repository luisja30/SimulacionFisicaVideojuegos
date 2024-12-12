#pragma once

#include <PxPhysicsAPI.h>
#include "../core.hpp"
#include "../RenderUtils.hpp"
#include <iostream>

using namespace physx;

class Platform {
protected:
	PxRigidStatic* rigidStatic_;
	RenderItem* renderItem_;
public:
	Platform(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, int x, int y, int z, Vector4 c);
	virtual ~Platform();
	Vector3 getPosition() const { return rigidStatic_->getGlobalPose().p; };
};

