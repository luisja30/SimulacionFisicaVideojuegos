#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Vector3 pos, double range)
	: origin_(pos), range_(range) {

	//Creamos las fuerzas
	forceRegistry_ = new ParticleForceRegistry();
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
	
	delete forceRegistry_;
}

void ParticleGenerator::createForceGenerators() {
	//Fuerza gravitatoria
	gF_ = new GravityForceGenerator(Vector3(0, -9.8, 0), "GRAVITY");
	invGF_ = new GravityForceGenerator(Vector3(0, 9.8, 0), "INVERSE_GRAVITY");

	//Viento
	windF_ = new WindForceGenerator(origin_, Vector3(0, 100, 0), 70.0f, 1.0f, 0.0f, "WIND");

	//Tornado
	tornadoF_ = new WhirlwindForceGenerator(origin_, 100.0f, 0.5f, "TORNADO");

	//Explosion
	explosionF_ = new ExplosionForceGenerator(origin_, 10000000, 1000, 900000, "EXPLOSION");


	bfg_ = nullptr;
	/*forceGen_.push_back(gF_);
	forceGen_.push_back(invGF_);
	forceGen_.push_back(windF_);
	forceGen_.push_back(tornadoF_);
	forceGen_.push_back(explosionF_);*/
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

void ParticleGenerator::createExplosion(int n) {
	for (int i = 0; i < n; i++) {
		Vector3 newPos = origin_, newVel = { 0,0,0 };
		int radius = 5;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		//Añadimos particula
		Particle* newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
		newParticle->setColor({ 1,0,0,1 });
		particles_.push_back(newParticle);

		forceRegistry_->addRegistry(explosionF_, newParticle);
	}
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
		forceRegistry_->addRegistry(gF_, newParticle);
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

	case TORNADO:
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
		forceRegistry_->addRegistry(tornadoF_, newParticle);
		break;
	}
}

void ParticleGenerator::update(double t) {
	if(genMode_ != SPRING_MODE && genMode_ != EXPLOSION)
		generateParticle();

	forceRegistry_->updateForces();

	for (auto it = particles_.begin(); it != particles_.end();) {
		if (*it != nullptr) {
			(*it)->integrate(t);
			//Comprobar rango de recorrido o si la particula sigue viva
			if (!(*it)->isAlive() || ((*it)->getPosition() - origin_).magnitude() > range_) {
				forceRegistry_->deleteParticleRegistry(*it);
				delete* it;
				it = particles_.erase(it);
			}
			else it++;
		}
	}
}

void ParticleGenerator::setMode(int i) {

	resetScene();

	switch (i) {
	//Efectos P2
	case 0:
		genMode_ = RAIN;
		break;
	case 1:
		genMode_ = MIST;
		break;
	case 2:
		genMode_ = HOSE;
		break;

	//Fuerzas P3
	case 3:
		genMode_ = TORNADO;
		break;
	case 4:
		genMode_ = EXPLOSION;
		createExplosion(500);
		break;

	//Muelles P4
	case 5:
		genMode_ = SPRING_MODE;
		generateSpringAnchoredDemo();
		break;
	case 6:
		genMode_ = SPRING_MODE;
		generateSpringDemo(false);
		break;
	case 7: 
		genMode_ = SPRING_MODE;
		//Goma elastica
		generateSpringDemo(true);
		break;
	case 8:
		genMode_ = SPRING_MODE;
		generateBuoyancyDemo();
		break;
	}
}

void ParticleGenerator::resetScene() {
	clearParticles();
	clearForces();
}

//Solamente limpia las particulas
void ParticleGenerator::clearParticles() {
	while (!particles_.empty()) {
		forceRegistry_->deleteParticleRegistry(particles_.front());
		delete particles_.front();
		particles_.pop_front();
	}
}

void ParticleGenerator::clearForces() {
	if (!forceGen_.empty()) {
		for (auto it = forceGen_.begin(); it != forceGen_.end(); it++) {
			delete* it;
			forceRegistry_->deleteForceRegistry(*it);
		}
		forceGen_.clear();
	}
}

void ParticleGenerator::generateSpringAnchoredDemo() {
    Particle* p1 = new Particle(Vector3(0, 45, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 2.0f, 100, 1.0, SPHERE);
	p1->setColor({ 1.0,0.0,0.0,1.0 });
	p1->setSemiEuler();

	Particle* p2 = new Particle(Vector3(5, 45, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 2.0f, 100, 1.0, SPHERE);
	p2->setColor({ 0.0,1.0,0.0,1.0 });
	p2->setSemiEuler();

	Particle* p3 = new Particle(Vector3(-5, 45, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 2.0f, 100, 1.0, SPHERE);
	p3->setColor({ 0.0,1.0,1.0,1.0 });
	p3->setSemiEuler();

	AnchoredSpringFG* fAnchor = new AnchoredSpringFG(1, 20, Vector3(0, 50, 0));

	forceRegistry_->addRegistry(fAnchor, p1);
	forceRegistry_->addRegistry(fAnchor, p2);
	forceRegistry_->addRegistry(fAnchor, p3);
	forceGen_.push_back(fAnchor);
	particles_.push_back(p1);
	particles_.push_back(p2);
	particles_.push_back(p3);
}

void ParticleGenerator::generateSpringDemo(bool elastic) {
	//Dos particulas unidas mediente un muelle
	Particle* p1 = new Particle(Vector3(50, 25, 0), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);
	Particle* p2 = new Particle(Vector3(-50, 25, 5), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);
	Particle* p3 = new Particle(Vector3(0, 50, -5), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);
	Particle* p4 = new Particle(Vector3(0, 0, -5), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);

	p1->setColor({ 0.0,0.0,1.0,1.0 });
	p2->setColor({ 0.0,1.0,0.0,1.0 });
	p3->setColor({ 1.0,0.0,0.0,1.0 });
	p4->setColor({ 1.0,1.0,0.0,1.0 });

	SpringForceGenerator* sfg1 = new SpringForceGenerator(Vector3(0, 25, 0), 4, 10, p2, elastic);
	SpringForceGenerator* sfg2 = new SpringForceGenerator(Vector3(0, 25, 0), 4, 10, p1, elastic);
	SpringForceGenerator* sfg3 = new SpringForceGenerator(Vector3(0, 25, 0), 4, 10, p4, elastic);
	SpringForceGenerator* sfg4 = new SpringForceGenerator(Vector3(0, 25, 0), 4, 10, p3, elastic);

	//GravityForceGenerator* gF = new GravityForceGenerator(Vector3(0, -9.8, 0), "GRAVITY");
	//WindForceGenerator* wfg = new WindForceGenerator(origin_, Vector3(0, 100, 0), 70.0f, 1.0f, 0.0f, "WIND");

	
	//forceRegistry_->addRegistry(gF, p1);
	//forceRegistry_->addRegistry(wfg, p1);
	//forceRegistry_->addRegistry(wfg, p3);
	forceRegistry_->addRegistry(sfg1, p1);
	forceRegistry_->addRegistry(sfg2, p2);
	forceRegistry_->addRegistry(sfg3, p3);
	forceRegistry_->addRegistry(sfg4, p4);

	//forceGen_.push_back(gF);
	//forceGen_.push_back(wfg);
	forceGen_.push_back(sfg1);
	forceGen_.push_back(sfg2);
	forceGen_.push_back(sfg3);
	forceGen_.push_back(sfg4);
	
	particles_.push_back(p1);
	particles_.push_back(p2);
	particles_.push_back(p3);
	particles_.push_back(p4);
}

void ParticleGenerator::generateBuoyancyDemo() {
	Particle* p1 = new Particle({ 0.0, 30, 0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, 3.0f, 100, 1.0, BOX);
	p1->setMass(1.0);
	p1->setColor({ 1.0,1.0,0.0,1.0 });

	bfg_ = new BuoyancyForceGenerator({ 0, 0 ,0 }, 3.0, 0.01, 1000);

	forceGen_.push_back(bfg_);
	particles_.push_back(p1);
	forceRegistry_->addRegistry(bfg_, p1);
	forceRegistry_->addRegistry(gF_, p1);
}

void ParticleGenerator::changeK(char k) {
	if (genMode_ == SPRING_MODE) {
		SpringForceGenerator* sfg = nullptr;
		ForceGenerator* fg = nullptr;
		int sign = k == '+' ? 1 : -1;

		auto it = forceGen_.begin();
		bool found = false;

		while (it != forceGen_.end() && !found) {
			fg = (*it);
			if (fg->getName() == "SPRING") {
				sfg = static_cast<SpringForceGenerator*>(fg);
				if (sfg->getK() + sign > 0)
					//k == '+' ? sfg->setK(sfg->getK() + 1) : sfg->setK(sfg->getK() - 1);
					sfg->setK(sfg->getK() + sign);

				found = true;
			}
			else it++;
		}
		if (found) {
			std::cout << "k "<< sfg->getK();
			k == '+' ? cout << " (+)\n" : cout << " (-)\n";
		}
	}
}

void ParticleGenerator::changeVolume(char v) {
	if (bfg_ != nullptr) {
		float currVolumen = bfg_->getVolume();
		int sign = v == '.' ? 1 : -1;

		if (bfg_->getVolume() + sign > 0) {
			currVolumen += sign;
			bfg_->setVolume(currVolumen);
			cout << "v: " << bfg_->getVolume();
			v == '.' ? cout << " (+)\n" : cout << " (-)\n";
		}
	}
}

void ParticleGenerator::changeMass(char m) {
	for (auto it = particles_.begin(); it != particles_.end();) {
		int sign = m == '0' ? 1 : -1;
		Particle* p = (*it);
		if (p->getMass() + sign > 0)
			p->setMass(p->getMass() + sign);

		cout << "m: " << p->getMass();
		m == '0' ? cout << " (+)\n" : cout << " (-)\n";

		it++;
	}
}




