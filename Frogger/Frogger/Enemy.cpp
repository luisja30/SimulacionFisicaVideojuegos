#include "Enemy.h"

Enemy::Enemy(PxPhysics* gPhysics, PxScene* gScene, PxShape* sp, Vector3 pos, double m, double v, Vector4 c)
	: RigidBody(gPhysics, gScene, sp, pos, m, v, c) {}
