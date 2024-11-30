#pragma once

#include "Particle.h"
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "BuoyancyForceGenerator.h"
#include "ParticleForceRegistry.h"
#include <random>
#include <list>

enum GenerateMode {
	RAIN,
	MIST,
	HOSE,
	WIND,
	TORNADO,
	EXPLOSION,
	SPRING_MODE
};

class ParticleGenerator {
protected:
	Vector3 origin_;
	std::list<Particle*> particles_;
	int range_, gravity_ = -9.8;
	std::mt19937_64 rd;
	GenerateMode genMode_;

	//Metodo abstarcto para generar particulas con distintas distribuciones
	virtual void generateParticle();

	//Fuerzas
	std::vector<ForceGenerator*> forceGen_;
	ParticleForceRegistry* forceRegistry_;

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
	ParticleGenerator(Vector3 pos, double range);
	virtual ~ParticleGenerator();

	virtual void update(double t);

	void createForceGenerators();
	ForceGenerator* getForceGenerator(string name);
	void createExplosion(int n);

	void setMode(int i);
	void resetScene();
	void clearParticles();
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

