#include "Platform.h"

Platform::Platform(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, int x, int y, int z, Vector4 c) {
	rigidStatic_ = gPhysics->createRigidStatic(PxTransform(pos));
	PxShape* shape = CreateShape(PxBoxGeometry(x, y, z));
	rigidStatic_->attachShape(*shape);
	gScene->addActor(*rigidStatic_);
	renderItem_ = new RenderItem(shape, rigidStatic_, c);
}

Platform::~Platform() {
	rigidStatic_->release();
	renderItem_->release();
	renderItem_ = nullptr;
}