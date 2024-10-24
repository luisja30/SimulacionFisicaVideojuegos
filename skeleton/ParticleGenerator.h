#pragma once

#include "Particle.h"
#include <random>
#include <list>

class ParticleGenerator {
protected:
	Vector3 origin_, vel_, dir_;
	std::list<Particle*> particles_;
	int range_, gravity_ = -9.8;
	std::normal_distribution<double> d{ 0,1 };
	std::mt19937_64 rd;
	//double frec_, currentFrec_;

	//Metodo abstarcto para generar particulas con distintas distribuciones
	virtual void generateParticle() = 0;

public:
	ParticleGenerator(Vector3 pos, double range);
	~ParticleGenerator();
	virtual void update(double t) = 0;

//Metodos
	//Update que haga integrate de cada particula
	//Crear generadores de particulas (clases abstractas) para distribucion normal y gaussiana
};

