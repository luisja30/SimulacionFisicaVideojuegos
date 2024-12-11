#pragma once

#include <PxPhysicsAPI.h>
#include "../core.hpp"
#include "../ActorSystem.h"
#include "../Frogger/Frogger.h"

/*Clase en el que se almacena todo lo que tenga que ver con el juego*/
class GameScene {
protected:
	const int WIDTH = 400;
	const int HEIGHT = 500;

	PxPhysics* gPhysics_;
	PxScene* gScene_;

	Frogger* player_;
	PxRigidStatic* floor_;
	RenderItem* renderItemFloor_;

public:
	GameScene(PxPhysics* gPhysics, PxScene* gScene);
	void initPlayer();
	virtual ~GameScene();
	void update(double t);

};

