#pragma once

#include "Particle.h"
#include <random>
#include <list>

class ParticleSystem {
private:
	Vector3 pos_, vel_, dir_;
	std::list<Particle*> particles_;
	int range_;
	//std::random_device rd;
	std::normal_distribution<double> d{ 0,1 };
	std::mt19937_64 rd;

	//double frec_, currentFrec_;

	void generateParticle();


public:
	ParticleSystem(Vector3 pos, Vector3 vel, Vector3 dir, double range);
	void update(double t);

//Metodos
	//Update que haga integrate de cada particula
	//Crear generadores de particulas (clases abstractas) para distribucion normal y gaussiana
};

