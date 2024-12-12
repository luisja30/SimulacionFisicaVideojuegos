#pragma once

//#include "Actor.h"
#include "Particle.h"
#include "RigidBody.h"
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "BuoyancyForceGenerator.h"
#include "ActorForceRegistry.h"
#include <random>
#include <list>

enum GenerateActorMode {
	RAIN_A,
	MIST_A,
	HOSE_A,
	WIND_A,
	TORNADO_A,
	EXPLOSION_A,
	SPRING_MODE_A,
	FROGGER_EXPLOSION,
	FROGGER_SMOKE
};
enum ActorMode {
	PARTICLES,
	RIGIDBODY
};

class ActorSystem {
	protected:
	Vector3 pos_;
	std::list<Actor*> actors_;
	int actorCount_ = 0, actorLimit_ = 150;
	double range_;

	std::mt19937_64 rd;
	GenerateActorMode genMode_;

	//ActorMode actorMode_ = PARTICLES;
	ActorMode actorMode_ = RIGIDBODY;

	PxPhysics* gPhysics_;
	PxScene* gScene_;

	void generateActor();

	//Fuerzas
	std::vector<ForceGenerator*> forceGen_;
	ActorForceRegistry* forceRegistry_;

	//Tipos de Fuerzas
	GravityForceGenerator* gF_;
	GravityForceGenerator* invGF_;
	WindForceGenerator* windF_;
	WhirlwindForceGenerator* tornadoF_;
	ExplosionForceGenerator* explosionF_;

	//Muelles
	AnchoredSpringFG* anchoredfg_;
	SpringForceGenerator* springfg_;
	BuoyancyForceGenerator* bfg_ = nullptr;
public:
	ActorSystem(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, double range); 
	ActorSystem(Vector3 pos, double range);
	virtual ~ActorSystem();

	virtual void update(double t);

	void createForceGenerators();
	ForceGenerator* getForceGenerator(string name);
	void createExplosion(int n);
	void createFroggerExplosion(int n);

	void keyPressed(char k);
	void changeActorMode(char k);
	void resetScene();
	void clearActors();
    void clearForces();


	//Setters
	void setActorMode(ActorMode aMode);
	void setGenMode(GenerateActorMode genMode);
	void setLimit(int l);
	//Muelles
	void generateSpringAnchoredDemo();
	void generateSpringDemo(bool elastic);
	void generateBuoyancyDemo();

	//Modificadores para muelles y particulas
	void changeK(char k);
	void changeVolume(char v);
	void changeMass(char m);
};

