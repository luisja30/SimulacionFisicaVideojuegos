#pragma once

#include "Particle.h"
#include <random>
#include <list>

class ParticleGenerator {
protected:
	Vector3 origin_;/*, vel_, dir_;*/
	std::list<Particle*> particles_;
	int range_, gravity_ = -9.8;
	std::mt19937_64 rd;
	//std::normal_distribution<double> d{ 0,1 };
	//double frec_, currentFrec_;

	//Metodo abstarcto para generar particulas con distintas distribuciones
	virtual void generateParticle() = 0;

public:
	ParticleGenerator(Vector3 pos, double range);
	virtual ~ParticleGenerator();
	virtual void update(double t) = 0;
};

