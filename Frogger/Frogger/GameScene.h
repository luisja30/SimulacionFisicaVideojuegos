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

	//Juagdor
	Frogger* player_;

	//Suelo
	PxRigidStatic* floor_;
	RenderItem* renderItemFloor_;

	//limites
	vector<Platform*> limits_;

	//Enemigos
	vector<Enemy*> enemies_;

	//Metas
	vector<Goal*> goals_;

	//Variables locales
	int currGoals_;
	bool endGame_;

public:
	GameScene(PxPhysics* gPhysics, PxScene* gScene);
	virtual ~GameScene(); //eliminacion de instancias de los objetos en escena 

	//Inicializadores
	void initPlayer();
	void initPlatforms();
	void initEnemies();
	void initGoals();


	//Colisiones con las metas
	void collisionsPlayerGoals();

	//Colisiones entre solidos rigidos (jugador y enemigos)
	void onCollision(PxActor* actor1, PxActor* actor2);

	//Resetea la posicion del player
	void resetPlayerPosition();

	void endGame();

	//Metodos que se llaman en main.cpp
	void update(double t);
	void keyPressed(char k);

};

