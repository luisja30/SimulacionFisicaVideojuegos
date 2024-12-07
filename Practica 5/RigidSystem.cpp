#include "RigidSystem.h"

RigidSystem::RigidSystem(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, double range) :
	gPhysics_(gPhysics), gScene_(gScene), pos_(pos), range_(range){

	forceRegistry_ = new RigidForceRegistry();
	createForceGenerators();
}

RigidSystem::~RigidSystem() {
	//Recorremos la lista y vamos eliminando las partículas para que no quede basura
	for (RigidBody* r : rigidBodies_)
		delete r;
	rigidBodies_.clear();

	for (ForceGenerator* fg : forceGen_)
		delete fg;
	forceGen_.clear();

	delete forceRegistry_;
}


void RigidSystem::createForceGenerators() {
	//Fuerza gravitatoria
	gF_ = new GravityForceGenerator(Vector3(0, -9.8, 0), "GRAVITY");
	invGF_ = new GravityForceGenerator(Vector3(0, 9.8, 0), "INVERSE_GRAVITY");

	//Viento
	windF_ = new WindForceGenerator(pos_, Vector3(0, 100, 0), 70.0f, 1.0f, 0.0f, "WIND");

	//Tornado
	tornadoF_ = new WhirlwindForceGenerator(pos_, 100.0f, 0.5f, "TORNADO");

	//Explosion
	explosionF_ = new ExplosionForceGenerator(pos_, 10000000, 1000, 900000, "EXPLOSION");


	bfg_ = nullptr;
	/*forceGen_.push_back(gF_);
	forceGen_.push_back(invGF_);
	forceGen_.push_back(windF_);
	forceGen_.push_back(tornadoF_);
	forceGen_.push_back(explosionF_);*/
}

void RigidSystem::update(double t) {
	if (genMode_ != SPRING_MODE_R && genMode_ != EXPLOSION_R) {
		if (rigidCount_ < rigidLimit_) {
			generateRigid();
		}
	}

	forceRegistry_->updateForces();

	for (auto it = rigidBodies_.begin(); it != rigidBodies_.end();) {
		if (*it != nullptr) {
			(*it)->integrate(t);
			//Comprobar rango de recorrido o si la particula sigue viva
			if (!(*it)->getAlive() || ((*it)->getPosition() - pos_).magnitude() > range_) {
				forceRegistry_->deleteRigidRegistry(*it);
				delete* it;
				it = rigidBodies_.erase(it);
				rigidCount_--;
			}
			else it++;
		}
	}

}


ForceGenerator* RigidSystem::getForceGenerator(string name) {
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

void RigidSystem::createExplosion(int n) {
	for (int i = 0; i < n; i++) {
		Vector3 newPos = pos_, newVel = { 0,0,0 };
		int radius = 5;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		//Añadimos particula
		//Particle* newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
		//newParticle->setColor({ 1,0,0,1 });
		RigidBody* newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(1, 0, 0, 1));
		rigidBodies_.push_back(newRigid);

		forceRegistry_->addRegistry(explosionF_, newRigid);
	}
}

void RigidSystem::generateRigid() {
	Vector3 newPos = pos_, newVel = { 0,0,0 };
	double radius = 0;
	double newLifeTime = 0;
	RigidBody* newRigid = nullptr;
	double newMass = 1;
	float size = 0;

	switch (genMode_) {
	case RAIN_R:
		radius = 100;
		//Generamos una posicion aletaria en X y Z
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y = pos_.y;
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		//Las particulas se mueven solo en el eje Y
		newVel.y = -300;

		//Añadimos particula
		newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(0, 0, 1, 1));
		newRigid->setVelocity(newVel);
		newRigid->setAliveTime(3);
		rigidBodies_.push_back(newRigid);

		//Adherimos una fuerza a la particula
		//forceRegistry_->addRegistry(gF_, newParticle);
		break;

	case MIST_R:
		radius = 50;
		newPos.x = std::normal_distribution<double>(pos_.x - radius, pos_.x + radius)(rd);
		newPos.y = std::normal_distribution<double>(pos_.y - radius, pos_.y + radius)(rd);
		newPos.z = std::normal_distribution<double>(pos_.z - radius, pos_.z + radius)(rd);

		newVel.x = std::uniform_real_distribution<double>(-1, 1)(rd);
		newVel.y = std::uniform_real_distribution<double>(-1, 1)(rd);
		newVel.z = std::uniform_real_distribution<double>(-1, 1)(rd);

		//Damos un tiempo de vida aleatorio
		newLifeTime = std::uniform_real_distribution<double>(9, 15)(rd);
		//Añadimos particula
		newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(1, 1, 1, 1));
		newRigid->setVelocity(newVel);
		newRigid->setAliveTime(newLifeTime);

		rigidBodies_.push_back(newRigid);

		break;

	case HOSE_R:
		newPos.x = pos_.x;
		newPos.y = pos_.y;
		newPos.z = pos_.z;

		newVel.x = std::uniform_real_distribution<double>(-10, 10)(rd);
		newVel.y = std::uniform_real_distribution<double>(-300, -200)(rd);
		newVel.z = std::uniform_real_distribution<double>(-10, 10)(rd);

		size = std::uniform_real_distribution<float>(1.0f, 6.0f)(rd);
		//Añadimos particula
		newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(size)), newPos, 1, 1, Vector4(0, 0, 1, 1));
		newRigid->setVelocity(newVel);
		newRigid->setAliveTime(2);
		rigidBodies_.push_back(newRigid);
		break;

	case WIND_R:
		radius = 10;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y = pos_.y;
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		//Añadimos particula
		//newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
		//newParticle->setColor({ 0,0,1,1 });
		newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(0, 0, 1, 1));
		newRigid->setVelocity(newVel); 
		newRigid->setAliveTime(100);
		rigidBodies_.push_back(newRigid);

		/*newMass = std::normal_distribution<double>(1.0, 20.0)(rd);
		newParticle->setMass(newMass);*/

		rigidBodies_.push_back(newRigid);

		forceRegistry_->addRegistry(windF_, newRigid);
		break;

	case TORNADO_R:
		radius = 10;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y = pos_.y;
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		//Las particulas se mueven solo en el eje Y
		newVel.y = -50;

		//Añadimos particula
		//newRigid = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
		//newParticle->setColor({ 0,1,1,1 });
		//particles_.push_back(newParticle);

		newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(0, 1, 1, 1));
		newRigid->setVelocity(newVel);
		newRigid->setAliveTime(100);
		rigidBodies_.push_back(newRigid);

		forceRegistry_->addRegistry(tornadoF_, newRigid);
		break;
	}
	rigidCount_++;
}

void RigidSystem::keyPressed(char k) {
	resetScene();
	switch (k) {
		case '1': {
			genMode_ = RAIN_R;
			break;
		}
		case '2': {
			genMode_ = MIST_R;
			break;
		}
		case '3': {
			genMode_ = HOSE_R;
			break;
		}
		//Fuerzas
		case '4': {
			genMode_ = TORNADO_R;
			break;
		}
		case '5': {
			genMode_ = EXPLOSION_R;
			break;
		}
		//Muelles
		case '6': {
			genMode_ = SPRING_MODE_R;
			break;
		}
		case '7': {
			genMode_ = SPRING_MODE_R;
			break;
		}
		case '8': {
			genMode_ = SPRING_MODE_R;
			break;
		}



	}
}

void RigidSystem::setMode(int i) {
	//resetScene();

	//switch (i) {
	//	//Efectos P2
	//case 0:
	//	genMode_ = RAIN;
	//	break;
	//case 1:
	//	genMode_ = MIST;
	//	break;
	//case 2:
	//	genMode_ = WIND;
	//	break;
	//}
}

void RigidSystem::resetScene() {
	clearRigidBodies();
	clearForces();
}

void RigidSystem::clearRigidBodies() {
	while (!rigidBodies_.empty()) {
		forceRegistry_->deleteRigidRegistry(rigidBodies_.front());
		delete rigidBodies_.front();
		rigidBodies_.pop_front();
	}
	rigidCount_ = 0;
}

void RigidSystem::clearForces() {
	if (!forceGen_.empty()) {
		for (auto it = forceGen_.begin(); it != forceGen_.end(); it++) {
			delete* it;
			forceRegistry_->deleteForceRegistry(*it);
		}
		forceGen_.clear();
	}
}

void RigidSystem::generateSpringAnchoredDemo() {
	/*Particle* p1 = new Particle(Vector3(0, 45, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 2.0f, 100, 1.0, SPHERE);
	p1->setColor({ 1.0,0.0,0.0,1.0 });
	p1->setSemiEuler();

	Particle* p2 = new Particle(Vector3(5, 45, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 2.0f, 100, 1.0, SPHERE);
	p2->setColor({ 0.0,1.0,0.0,1.0 });
	p2->setSemiEuler();

	Particle* p3 = new Particle(Vector3(-5, 45, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 2.0f, 100, 1.0, SPHERE);
	p3->setColor({ 0.0,1.0,1.0,1.0 });
	p3->setSemiEuler();*/

	RigidBody* rb1 = new RigidBody(gPhysics_, gScene_,
		CreateShape(PxSphereGeometry(2)), Vector3(0, 45, 0), 1, 1, Vector4(1, 0, 0, 1));
	RigidBody* rb2 = new RigidBody(gPhysics_, gScene_,
		CreateShape(PxSphereGeometry(2)), Vector3(5, 45, 0), 1, 1, Vector4(0, 1, 0, 1));
	RigidBody* rb3 = new RigidBody(gPhysics_, gScene_,
		CreateShape(PxSphereGeometry(2)), Vector3(-5, 45, 0), 1, 1, Vector4(0, 1, 1, 1));

	

	AnchoredSpringFG* fAnchor = new AnchoredSpringFG(1, 20, Vector3(0, 50, 0));

	forceRegistry_->addRegistry(fAnchor, rb1);
	forceRegistry_->addRegistry(fAnchor, rb2);
	forceRegistry_->addRegistry(fAnchor, rb3);
	forceGen_.push_back(fAnchor);
	rigidBodies_.push_back(rb1);
	rigidBodies_.push_back(rb2);
	rigidBodies_.push_back(rb3);
}

void RigidSystem::generateSpringDemo(bool elastic) {
	//Dos particulas unidas mediente un muelle
	//Particle* p1 = new Particle(Vector3(20, 10, 0), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);
	//Particle* p2 = new Particle(Vector3(0, 10, 0), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);
	//Particle* p3 = new Particle(Vector3(0, 50, -5), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);
	//Particle* p4 = new Particle(Vector3(0, 0, -5), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);

	//p1->setColor({ 0.0,0.0,1.0,1.0 });
	//p2->setColor({ 0.0,1.0,0.0,1.0 });
	//p3->setColor({ 1.0,0.0,0.0,1.0 });
	//p4->setColor({ 1.0,1.0,0.0,1.0 });

	//p2->setMass(2.0);

	RigidBody* rb1 = new RigidBody(gPhysics_, gScene_,
		CreateShape(PxSphereGeometry(2)), Vector3(20, 10, 0), 1, 1, Vector4(0, 0, 1, 1));
	RigidBody* rb2 = new RigidBody(gPhysics_, gScene_,
		CreateShape(PxSphereGeometry(2)), Vector3(0, 10, 0), 1, 1, Vector4(0, 1, 0, 1));
	RigidBody* rb3 = new RigidBody(gPhysics_, gScene_,
		CreateShape(PxSphereGeometry(2)), Vector3(0, 50, -5), 1, 1, Vector4(1, 0, 0, 1));
	RigidBody* rb4 = new RigidBody(gPhysics_, gScene_,
		CreateShape(PxSphereGeometry(2)), Vector3(0, 0, -5), 1, 1, Vector4(0, 1, 0, 1));


	SpringForceGenerator* sfg1 = new SpringForceGenerator(Vector3(0, 10, 0), 1, 10, rb2, elastic);
	SpringForceGenerator* sfg2 = new SpringForceGenerator(Vector3(0, 10, 0), 1, 10, rb1, elastic);
	SpringForceGenerator* sfg3 = new SpringForceGenerator(Vector3(0, 25, 0), 1, 30, rb4, elastic);
	SpringForceGenerator* sfg4 = new SpringForceGenerator(Vector3(0, 25, 0), 1, 30, rb3, elastic);

	//GravityForceGenerator* gF = new GravityForceGenerator(Vector3(0, -9.8, 0), "GRAVITY");
	//WindForceGenerator* wfg = new WindForceGenerator(origin_, Vector3(0, 100, 0), 70.0f, 1.0f, 0.0f, "WIND");


	//forceRegistry_->addRegistry(gF, p1);
	//forceRegistry_->addRegistry(wfg, p1);
	//forceRegistry_->addRegistry(wfg, p3);
	forceRegistry_->addRegistry(sfg1, rb1);
	forceRegistry_->addRegistry(sfg2, rb2);
	forceRegistry_->addRegistry(sfg3, rb3);
	forceRegistry_->addRegistry(sfg4, rb4);

	//forceGen_.push_back(gF);
	//forceGen_.push_back(wfg);
	forceGen_.push_back(sfg1);
	forceGen_.push_back(sfg2);
	forceGen_.push_back(sfg3);
	forceGen_.push_back(sfg4);

	rigidBodies_.push_back(rb1);
	rigidBodies_.push_back(rb2);
	rigidBodies_.push_back(rb3);
	rigidBodies_.push_back(rb4);
}

void RigidSystem::generateBuoyancyDemo() {
	Particle* p1 = new Particle({ 0.0, 20, 0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, 3.0f, 100, 1.0, BOX);
	p1->setMass(500);
	p1->setSemiEuler();
	p1->setColor({ 1.0,1.0,0.0,1.0 }); //Amarilla

	Particle* p2 = new Particle({ 0.0, 20, -10.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, 3.0f, 100, 1.0, BOX);
	p2->setMass(100);
	p2->setSemiEuler();
	p2->setColor({ 1.0,0.0,0.0,1.0 }); //Roja

	Particle* p3 = new Particle({ 0.0, 20, 10.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, 3.0f, 100, 1.0, BOX);
	p3->setMass(1000);
	p3->setSemiEuler();
	p3->setColor({ 1.0,0.0,1.0,1.0 }); //Morada

	RigidBody* rb1 = new RigidBody(gPhysics_, gScene_,
		CreateShape(PxBoxGeometry(3, 3, 3)), Vector3(0, 20, 0), 1, 1, Vector4(1, 1, 0, 1));
	RigidBody* rb2 = new RigidBody(gPhysics_, gScene_,
		CreateShape(PxBoxGeometry(3, 3, 3)), Vector3(0, 20, -10), 1, 1, Vector4(1, 0, 0, 1));
	RigidBody* rb3 = new RigidBody(gPhysics_, gScene_,
		CreateShape(PxBoxGeometry(3, 3, 3)), Vector3(0, 20, 10), 1, 1, Vector4(1, 0, 1, 1));


	bfg_ = new BuoyancyForceGenerator({ 0, 0 ,0 }, 0.0, 1.0, 1000);

	forceGen_.push_back(bfg_);
	rigidBodies_.push_back(rb1);
	rigidBodies_.push_back(rb2);
	rigidBodies_.push_back(rb3);

	forceRegistry_->addRegistry(bfg_, rb1);
	//forceRegistry_->addRegistry(gF_, p1);

	forceRegistry_->addRegistry(bfg_, rb2);
	//forceRegistry_->addRegistry(gF_, p2);

	forceRegistry_->addRegistry(bfg_, rb3);
	//forceRegistry_->addRegistry(gF_, p3);
}

void RigidSystem::changeK(char k) {
	if (genMode_ == SPRING_MODE_R) {
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
			std::cout << "k " << sfg->getK();
			k == '+' ? cout << " (+)\n" : cout << " (-)\n";
		}
	}
}

void RigidSystem::changeVolume(char v) {
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

void RigidSystem::changeMass(char m) {
	for (auto it = rigidBodies_.begin(); it != rigidBodies_.end();) {
		int sign = m == '0' ? 1 : -1;
		RigidBody* r = (*it);
		if (r->getMass() + sign > 0)
			r->setMass(r->getMass() + sign);

		cout << "m: " << r->getMass();
		m == '0' ? cout << " (+)\n" : cout << " (-)\n";

		it++;
	}
}
