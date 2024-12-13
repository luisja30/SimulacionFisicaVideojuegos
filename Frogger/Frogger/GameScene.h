#pragma once

#include <PxPhysicsAPI.h>
#include "../core.hpp"
#include "../ActorSystem.h"
#include "../Frogger/Frogger.h"
#include "../Frogger/Platform.h"
#include "../Frogger/Enemy.h"
#include "../Frogger/Car.h"
#include "../Frogger/Dock.h"
#include "../Frogger/Goal.h"

/* Clase en el que se almacena todo lo que tenga que ver con el juego */
class GameScene {
protected:
	const int WIDTH = 400;
	const int HEIGHT = 500;
	const int GOALS = 5;

	PxPhysics* gPhysics_;
	PxScene* gScene_;

	/* Elementos del juego */
	Frogger* player_;
	PxRigidStatic* floor_;
	RenderItem* renderItemFloor_;

	//Platform* floor_;
	vector<Platform*> limits_;

	//Coches
	vector<Enemy*> enemies_;
	vector<Goal*> goals_;

public:
	GameScene(PxPhysics* gPhysics, PxScene* gScene);
	virtual ~GameScene();

	//Inicializadores
	void initPlayer();
	void initPlatforms();
	void initEnemies();
	void initGoals();


	//Colisiones
	void collisionsPlayerGoals();


	void resetPlayerPosition();


	void update(double t);

	void keyPressed(char k);

};

