#pragma once

#include "Particle.h"
#include <list>

class ParticleSystem {
private:
	Vector3 pos_, vel_, dir_;
	std::list<Particle*> particles_;
	int range_;
	double frec_;

public:
	ParticleSystem(Vector3 pos, Vector3 vel, Vector3 dir);
	void update(double t);

//Metodos
	//Update que haga integrate de cada particula
	//Crear generadores de particulas (clases abstractas) para distribucion normal y gaussiana
};

