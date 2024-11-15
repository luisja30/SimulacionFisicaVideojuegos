#pragma once

#include "Particle.h"
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "ParticleForceRegistry.h"
#include <random>
#include <list>

enum GenerateMode {
	RAIN,
	MIST,
	HOSE,
	DEFAULT,
	DEFAULT_2
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
	ParticleForceRegistry* forceReg_;

	GravityForceGenerator* gF_;
	GravityForceGenerator* invGF_;
	WindForceGenerator* windF_;
	WhirlwindForceGenerator* tornadoF_;
	ExplosionForceGenerator* explosionF_;

public:
	ParticleGenerator(Vector3 pos, double range);
	virtual ~ParticleGenerator();

	void createForceGenerators();
	ForceGenerator* getForceGenerator(string name);
	virtual void update(double t);

	void setMode(int i);
	void clearPaticles();
};

