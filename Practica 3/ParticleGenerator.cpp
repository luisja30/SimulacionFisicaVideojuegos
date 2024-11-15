#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Vector3 pos, double range)
	: origin_(pos), range_(range) {

	//Creamos las fuerzas
	forceReg_ = new ParticleForceRegistry();
	createForceGenerators();
}

ParticleGenerator::~ParticleGenerator() {
	//Recorremos la lista y vamos eliminando las partículas para que no quede basura
	for (Particle* p : particles_)
		delete p;
	particles_.clear();

	for (ForceGenerator* fg : forceGen_)
		delete fg;
	forceGen_.clear();
	
	delete forceReg_;
}

void ParticleGenerator::createForceGenerators() {
	//Fuerza gravitatoria
	gF_ = new GravityForceGenerator(Vector3(0, -9.8, 0), "GRAVITY");
	invGF_ = new GravityForceGenerator(Vector3(0, 9.8, 0), "INVERSE_GRAVITY");

	//Viento
	windF_ = new WindForceGenerator(origin_, Vector3(50, 100, 0), 70.0f, 1.0f, 0.0f, "WIND");

	//Tornado
	tornadoF_ = new WhirlwindForceGenerator(origin_, 100.0f, 0.5f, "TORNADO");

	//Explosion
	explosionF_ = new ExplosionForceGenerator(origin_, 10000000, 1000, 900000, "EXPLOSION");

	forceGen_.push_back(gF_);
	forceGen_.push_back(invGF_);
	forceGen_.push_back(windF_);
	forceGen_.push_back(tornadoF_);
	forceGen_.push_back(explosionF_);
}

ForceGenerator* ParticleGenerator::getForceGenerator(string name) {
	auto it = forceGen_.begin();
	bool found = false;
	ForceGenerator* gen = nullptr;

	while (it != forceGen_.end() && !found) {
		if ((*it)->getName() == name) {
			gen = (*it);
			found = true;
		}
		else it++;
	}
	return gen;
}

void ParticleGenerator::generateParticle() {
	Vector3 newPos = origin_ , newVel = { 0,0,0 };
	double radius = 0;
	double newLifeTime = 0;
	Particle* newParticle = nullptr;

	switch (genMode_) {
	case RAIN:
		radius = 100;
		//Generamos una posicion aletaria en X y Z
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y = origin_.y;
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);
		//Vector3 newPos = Vector3(posX, posY, posZ);

		//Las particulas se mueven solo en el eje Y
		newVel.y = -300;

		//Añadimos particula
		newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
		newParticle->setColor({ 0,0,1,1 });
		particles_.push_back(newParticle);

		//Adherimos una fuerza a la particula
		forceReg_->addRegistry(gF_, newParticle);
		break;

	case MIST:
		radius = 50;
		newPos.x = std::normal_distribution<double>(origin_.x - radius, origin_.x + radius)(rd);
		newPos.y = std::normal_distribution<double>(origin_.y - radius, origin_.y + radius)(rd);
		newPos.z = std::normal_distribution<double>(origin_.z - radius, origin_.z + radius)(rd);
		//Vector3 newPos = Vector3(posX, posY, posZ);

		newVel.x = std::uniform_real_distribution<double>(-1, 1)(rd);
		newVel.y = std::uniform_real_distribution<double>(-1, 1)(rd);
		newVel.z = std::uniform_real_distribution<double>(-1, 1)(rd);
		//Vector3 newVel = Vector3(velX, velY, velZ);

		//Damos un tiempo de vida aleatorio
		newLifeTime = std::uniform_real_distribution<double>(9, 15)(rd);
		//Añadimos particula
		newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1.0f, newLifeTime, 10.0);
		newParticle->setColor({ 1.0,1.0,1.0,1.0 });
		particles_.push_back(newParticle);

		break;

	case HOSE:
		newPos.x = origin_.x;
		newPos.y = origin_.y;
		newPos.z = origin_.z;
		//Vector3 newPos = Vector3(posX, posY, posZ);

		newVel.x = std::uniform_real_distribution<double>(-10, 10)(rd);
		newVel.y = std::uniform_real_distribution<double>(-300, -200)(rd);
		newVel.z = std::uniform_real_distribution<double>(-10, 10)(rd);
		//Vector3 newVel = Vector3(velX, velY, velZ);

		//float size = std::uniform_real_distribution<float>(1.0f, 6.0f)(rd);
		//Añadimos particula
		newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 5.0f, 100, 10.0);
		newParticle->setColor({ 0,0,1,1 });
		particles_.push_back(newParticle);
		break;
	case DEFAULT:
		radius = 10;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y = origin_.y;
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		//Las particulas se mueven solo en el eje Y
		newVel.y = -50;

		//Añadimos particula
		newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
		newParticle->setColor({ 0,0,1,1 });
		particles_.push_back(newParticle);

		//forceReg_->addRegistry(windF_, newParticle);
		forceReg_->addRegistry(tornadoF_, newParticle);
		break;

	case DEFAULT_2:
		//Para explosion
		radius = 5;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		//Añadimos particula
		newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
		newParticle->setColor({ 1,0,0,1 });
		particles_.push_back(newParticle);

		forceReg_->addRegistry(explosionF_, newParticle);

		break;

	}
}

void ParticleGenerator::update(double t) {
	generateParticle();
	forceReg_->updateForces();
	for (auto it = particles_.begin(); it != particles_.end(); it++) {
		if (*it != nullptr) {
			(*it)->integrate(t);
			//Comprobar rango de recorrido o si la particula sigue viva
			if (!(*it)->isAlive() || ((*it)->getPosition() - origin_).magnitude() > range_) {
				forceReg_->deleteParticleRegistry(*it);
				delete* it;
				it = particles_.erase(it);
			}
		}
	}
}

void ParticleGenerator::setMode(int i) {
	switch (i) {
	case 0:
		genMode_ = RAIN;
		break;
	case 1:
		genMode_ = MIST;
		break;
	case 2:
		genMode_ = HOSE;
		break;
	case 3:
		genMode_ = DEFAULT;
		break;
	case 4:
		genMode_ = DEFAULT_2;
		break;
	}
	clearPaticles();
}

//Solamente limpia las particulas
void ParticleGenerator::clearPaticles() {
	while (!particles_.empty()) {
		forceReg_->deleteParticleRegistry(particles_.front());
		delete particles_.front();
		particles_.pop_front();
	}
}




