#pragma once

#include "Particle.h"
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include "ParticleForceRegistry.h"
#include <random>
#include <list>

enum TypeForce {
	GRAVITY,
	INVERSE_GRAVITY,
	WIND,
	TORNADO,
	EXPLOSION
};

class ParticleGenerator {
protected:
	Vector3 origin_;
	std::list<Particle*> particles_;
	int range_, gravity_ = -9.8;
	std::mt19937_64 rd;

	//Metodo abstarcto para generar particulas con distintas distribuciones
	virtual void generateParticle() = 0;

	//Fuerzas
	std::vector<ForceGenerator*> forceGen_;
	ParticleForceRegistry* forceReg_;

public:
	ParticleGenerator(Vector3 pos, double range);
	virtual ~ParticleGenerator();

	void createForceGenerators();

	ForceGenerator* getForceGenerator(string name);

	virtual void update(double t) = 0;
};

