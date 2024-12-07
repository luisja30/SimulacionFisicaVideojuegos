#pragma once

#include "RigidBody.h"
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "BuoyancyForceGenerator.h"
//#include "ParticleForceRegistry.h"
#include "RigidForceRegistry.h"
#include <random>
#include <list>

enum GenerateRigidMode {
	RAIN_R,
	MIST_R,
	HOSE_R,
	WIND_R,
	TORNADO_R,
	EXPLOSION_R,
	SPRING_MODE_R
};

class RigidSystem {
protected:
	Vector3 pos_;
	std::list<RigidBody*> rigidBodies_;
	int rigidCount_ = 0, rigidLimit_ = 500;
	double range_;

	std::mt19937_64 rd;
	GenerateRigidMode genMode_;

	PxPhysics* gPhysics_;
	PxScene* gScene_;

	void generateRigid();

	//Fuerzas
	std::vector<ForceGenerator*> forceGen_;
	RigidForceRegistry* forceRegistry_;

	//Tipos de Fuerzas
	GravityForceGenerator* gF_;
	GravityForceGenerator* invGF_;
	WindForceGenerator* windF_;
	WhirlwindForceGenerator* tornadoF_;
	ExplosionForceGenerator* explosionF_;

	//Muelles
	AnchoredSpringFG* anchoredfg_;
	SpringForceGenerator* springfg_;
	BuoyancyForceGenerator* bfg_;
public:
	RigidSystem(PxPhysics* gPhysics, PxScene* gScene,Vector3 pos, double range);
	virtual ~RigidSystem();

	virtual void update(double t);

	void createForceGenerators();
	ForceGenerator* getForceGenerator(string name);
	void createExplosion(int n);

	void keyPressed(char k);
	void setMode(int i);
	void resetScene();
	void clearRigidBodies();
    void clearForces();

	//Muelles
	void generateSpringAnchoredDemo();
	void generateSpringDemo(bool elastic);
	void generateBuoyancyDemo();

	//Modificadores para muelles y particulas
	void changeK(char k);
	void changeVolume(char v);
	void changeMass(char m);


};

