#pragma once

#include "Particle.h"
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include <random>
#include <list>

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

public:
	ParticleGenerator(Vector3 pos, double range);
	virtual ~ParticleGenerator();

	void createForceGenerators();
	virtual void update(double t) = 0;
};

