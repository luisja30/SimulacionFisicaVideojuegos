#include "BuoyancyForceGenerator.h"

BuoyancyForceGenerator::BuoyancyForceGenerator(Vector3 pos, float h, float V, float d) :
	ForceGenerator(pos, "BUOYANCY"), height_(h), volume_(V), liquid_density_(d) {

	//Superficie de flotación
	liquid_particle = new Particle(pos, { 0,0,0 }, { 0,0,0 }, 3, 1000, 1.0, PLANE);
	liquid_particle->setColor({ 0.0,1.0,1.0,1.0 });
}


BuoyancyForceGenerator::~BuoyancyForceGenerator() {
	delete liquid_particle;
}

void BuoyancyForceGenerator::updateForce(Actor* a){
	float h = a->getPosition().y;
	float h0 = liquid_particle->getPosition().y;

	Vector3 f(0, 0, 0);
	float immersed = 0.0;

	if (h - h0 > height_ * 0.5)
		immersed = 0.0;

	else if (h0 - h > height_ * 0.5)
		//Totalmente inmerso
		immersed = 1.0;
	else
		immersed = (h0 - h) / height_ + 0.5;

	f.y = liquid_density_ * volume_ * immersed * gravity_;
	a->addForce(f);
}

