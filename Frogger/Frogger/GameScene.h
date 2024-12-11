#pragma once

#include <PxPhysicsAPI.h>
#include "../core.hpp"
#include "../ActorSystem.h"
#include "../Frogger/Frogger.h"
#include "../Frogger/Platform.h"

/* Clase en el que se almacena todo lo que tenga que ver con el juego */
class GameScene {
protected:
	const int WIDTH = 400;
	const int HEIGHT = 500;

	PxPhysics* gPhysics_;
	PxScene* gScene_;

	/* Elementos del juego */
	Frogger* player_;
	PxRigidStatic* floor_;
	RenderItem* renderItemFloor_;

	//Platform* floor_;
	vector<Platform*> limits_;

public:
	GameScene(PxPhysics* gPhysics, PxScene* gScene);
	virtual ~GameScene();

	void initPlayer();
	void initPlatforms();

	void update(double t);
	void keyPressed(char k);

};

