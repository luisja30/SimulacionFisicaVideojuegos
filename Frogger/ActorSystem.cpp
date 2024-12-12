#include "ActorSystem.h"

ActorSystem::ActorSystem(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, double range) :
	gPhysics_(gPhysics), gScene_(gScene), pos_(pos), range_(range) {

	forceRegistry_ = new ActorForceRegistry();
	createForceGenerators();
}

ActorSystem::~ActorSystem(){
	//Recorremos la lista y vamos eliminando las partículas para que no quede basura
	for (Actor* a : actors_)
		delete a;
	actors_.clear();

	for (ForceGenerator* fg : forceGen_)
		delete fg;
	forceGen_.clear();

	delete forceRegistry_;
}

void ActorSystem::createForceGenerators() {
	//Fuerza gravitatoria
	gF_ = new GravityForceGenerator(Vector3(0, -9.8, 0), "GRAVITY");
	invGF_ = new GravityForceGenerator(Vector3(0, 9.8, 0), "INVERSE_GRAVITY");

	//Viento
	windF_ = new WindForceGenerator(pos_, Vector3(15, 100, 0), 70.0f, 1.0f, 0.0f, "WIND");

	//Tornado
	tornadoF_ = new WhirlwindForceGenerator(pos_, 100.0f, 0.5f, "TORNADO");

	//Explosion
	explosionF_ = new ExplosionForceGenerator(pos_, 10000000, 1000, 900000, "EXPLOSION");
}

void ActorSystem::update(double t) {
	if (genMode_ != SPRING_MODE_A 
		&& genMode_ != EXPLOSION_A
		&& genMode_ != FROGGER_EXPLOSION) {
		if (actors_.size() < actorLimit_) {
			generateActor();
		}
	}

	forceRegistry_->updateForces();

	for (auto it = actors_.begin(); it != actors_.end();) {
		if (*it != nullptr) {
			//Comprobar rango de recorrido o si la particula sigue viva
			if (!(*it)->integrate(t) || ((*it)->getPosition() - pos_).magnitude() > range_) {
				forceRegistry_->deleteActorRegistry(*it);
				delete* it;
				it = actors_.erase(it);
				actorCount_--;
			}
			else it++;
		}
	}
}

ForceGenerator* ActorSystem::getForceGenerator(string name){
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

void ActorSystem::createExplosion(int n) {
	for (int i = 0; i < n; i++) {
		Vector3 newPos = pos_, newVel = { 0,0,0 };
		int radius = 5;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		if (actorMode_ == PARTICLES) {
			//Añadimos particula
			Particle* newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
			newParticle->setColor({ 1,0,0,1 });
			actors_.push_back(newParticle);

			forceRegistry_->addRegistry(explosionF_, newParticle);
		}
		else {
			RigidBody* newRigid = new RigidBody(gPhysics_, gScene_, 
				CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(1, 0, 0, 1));
			actors_.push_back(newRigid);

			forceRegistry_->addRegistry(explosionF_, newRigid);
		}
		actorCount_++;
	}
}

void ActorSystem::createFroggerExplosion(int n) {
	for (int i = 0; i < n; i++) {
		Vector3 newPos = pos_, newVel = { 0,0,0 };
		int radius = 5;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		double r = std::uniform_real_distribution<double>(0.3, 1.0)(rd);
		double g = std::uniform_real_distribution<double>(1.0, 1.0)(rd);
		double b = std::uniform_real_distribution<double>(0.3, 1.0)(rd);

		Vector4 newColor = Vector4(r, g, b, 1);

		if (actorMode_ == PARTICLES) {
			//Añadimos particula
			Particle* newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
			newParticle->setColor(newColor);
			actors_.push_back(newParticle);

			forceRegistry_->addRegistry(explosionF_, newParticle);
		}
		else {
			RigidBody* newRigid = new RigidBody(gPhysics_, gScene_,
				CreateShape(PxSphereGeometry(1)), newPos, 1, 1, newColor);
			actors_.push_back(newRigid);

			forceRegistry_->addRegistry(explosionF_, newRigid);
		}
		actorCount_++;
	}

}

void ActorSystem::generateActor() {
	Vector3 newPos = pos_, newVel = { 0,0,0 };
	double radius = 0;
	double newLifeTime = 0;
	RigidBody* newRigid = nullptr;
	Particle* newParticle = nullptr;
	double newMass = 1;
	float size = 0;

	switch (genMode_) {
	case RAIN_A:
		radius = 100;
		//Generamos una posicion aletaria en X y Z
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y = pos_.y;
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		//Las particulas se mueven solo en el eje Y
		newVel.y = -300;

		if (actorMode_ == PARTICLES) {
			//Añadimos particula
			newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
			newParticle->setColor({ 0,0,1,1 });
			actors_.push_back(newParticle);

			//Adherimos una fuerza a la particula
			forceRegistry_->addRegistry(gF_, newParticle);
		}
		else {
			//Añadimos rigido
			newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(0, 0, 1, 1));
			newRigid->setVelocity(newVel);
			newRigid->setAliveTime(3);
			actors_.push_back(newRigid);

			//Adherimos una fuerza al rigido
			//forceRegistry_->addRegistry(gF_, newParticle);
		}

		break;

	case MIST_A:
		radius = 50;
		newPos.x = std::normal_distribution<double>(pos_.x - radius, pos_.x + radius)(rd);
		newPos.y = std::normal_distribution<double>(pos_.y - radius, pos_.y + radius)(rd);
		newPos.z = std::normal_distribution<double>(pos_.z - radius, pos_.z + radius)(rd);

		newVel.x = std::uniform_real_distribution<double>(-1, 1)(rd);
		newVel.y = std::uniform_real_distribution<double>(-1, 1)(rd);
		newVel.z = std::uniform_real_distribution<double>(-1, 1)(rd);

		//Damos un tiempo de vida aleatorio
		newLifeTime = std::uniform_real_distribution<double>(5, 10)(rd);

		if (actorMode_ == PARTICLES) {
			//Añadimos particula
			newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1.0f, newLifeTime, 10.0);
			newParticle->setColor({ 1.0,1.0,1.0,1.0 });
			actors_.push_back(newParticle);
		}
		else {
			//Añadimos particula
			newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(1, 1, 1, 1));
			newRigid->setVelocity(newVel);
			newRigid->setAliveTime(newLifeTime);
			newRigid->disableGravity();

			actors_.push_back(newRigid);
		}
		break;

	case HOSE_A:
		newPos.x = pos_.x;
		newPos.y = pos_.y;
		newPos.z = pos_.z;

		newVel.x = std::uniform_real_distribution<double>(-10, 10)(rd);
		newVel.y = std::uniform_real_distribution<double>(-300, -200)(rd);
		newVel.z = std::uniform_real_distribution<double>(-10, 10)(rd);

		size = std::uniform_real_distribution<float>(1.0f, 6.0f)(rd);

		if (actorMode_ == PARTICLES) {
			//Añadimos particula
			newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 5.0f, 100, 10.0);
			newParticle->setColor({ 0,0,1,1 });
			actors_.push_back(newParticle);
		}
		else {
			//Añadimos particula
			newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(size)), newPos, 1, 1, Vector4(0, 0, 1, 1));
			newRigid->setVelocity(newVel);
			newRigid->setAliveTime(2);
			actors_.push_back(newRigid);
		}
		break;

	case WIND_A:
		radius = 10;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y = pos_.y;
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		newMass = std::uniform_real_distribution<double>(2.0, 10.0)(rd);

		if (actorMode_ == PARTICLES) {
			//Añadimos particula
			newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
			newParticle->setColor({ 0,0,1,1 });
			newParticle->setMass(newMass);
			actors_.push_back(newParticle);

			forceRegistry_->addRegistry(windF_, newParticle);
		}
		else {
			newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(0, 0, 1, 1));
			newRigid->setAliveTime(100);
			newRigid->setMass(newMass);
			actors_.push_back(newRigid);

			forceRegistry_->addRegistry(windF_, newRigid);
		}
		break;

	case TORNADO_A:
		radius = 10;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y = pos_.y;
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		//Las particulas se mueven solo en el eje Y
		newVel.y = -50;

		if (actorMode_ == PARTICLES) {
			//Añadimos particula
			newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
			newParticle->setColor({ 0,1,1,1 });
			actors_.push_back(newParticle);

			forceRegistry_->addRegistry(tornadoF_, newParticle);
		}
		else {
			newRigid = new RigidBody(gPhysics_, gScene_, 
				CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(0, 1, 1, 1));
			newRigid->setVelocity(newVel);
			newRigid->setAliveTime(100);
			actors_.push_back(newRigid);

			forceRegistry_->addRegistry(tornadoF_, newRigid);
		}
		break;

	case FROGGER_SMOKE:
		radius = 3;
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y = pos_.y;
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

		//Las particulas se mueven solo en el eje Y
		//newVel.y = -50;

		//Añadimos una masa distinta a cada particula/rigido
		newMass = std::uniform_real_distribution<double>(2.0, 20.0)(rd);

		Vector4 colorGris = Vector4(0.5, 0.5, 0.5, 1);
		if (actorMode_ == PARTICLES) {
			//Añadimos particula
			newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 2, 100, newMass);
			newParticle->setColor(colorGris);
			actors_.push_back(newParticle);

			forceRegistry_->addRegistry(windF_, newParticle);
		}
		else {
			newRigid = new RigidBody(gPhysics_, gScene_,
				CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(0, 1, 1, 1));
			newRigid->setVelocity(newVel);
			newRigid->setAliveTime(100);
			newRigid->setColor(colorGris);
			actors_.push_back(newRigid);

			forceRegistry_->addRegistry(windF_, newRigid);
		}
		break;
	}

	actorCount_++;
}
void ActorSystem::keyPressed(char k) {
	switch (toupper(k)) {
	case '1': {
		resetScene();
		genMode_ = RAIN_A;
		break;
	}
	case '2': {   
		resetScene();
		genMode_ = MIST_A;
		break;
	}
	case '3': {
		resetScene();
		genMode_ = FROGGER_SMOKE;
		break;
	}
	//Fuerzas
	case '4': {
		resetScene();
		genMode_ = TORNADO_A;
		break;
	}
	case '5': {
		resetScene();
		genMode_ = FROGGER_EXPLOSION;
		//createExplosion(500);
		createFroggerExplosion(200);
		break;
	}
	//Muelles
	case '6': {
		resetScene();
		genMode_ = SPRING_MODE_A;
		generateSpringAnchoredDemo();
		break;
	}
	case '7': {
		resetScene();
		genMode_ = SPRING_MODE_A;
		generateSpringDemo(false);
		break;
	}
	case '8': {
		resetScene();
		genMode_ = SPRING_MODE_A;
		generateBuoyancyDemo();
		break;
	}
	case '0': {
		changeMass('0');
		break;
	}
	case '9': {
		changeMass('9');
		break;
	}
	case '-': {
		changeK('-');
		break;
	}
	case '+': {
		changeK('+');
		break;
	}
	case '.': {
		changeVolume('.');
		break;
	}
	case ',': {
		changeVolume(',');
		break;
	}
	//Cambiar actores
	case 'R':
		resetScene();
		actorMode_ = RIGIDBODY;
		break;
	case 'P':
		resetScene();
		actorMode_ = PARTICLES;
		break;
	//Desactivar fuerzas
	case 'T':
		//resetScene();
		tornadoF_->setActive(!tornadoF_->getActive());
		break;
	}
}

void ActorSystem::changeActorMode(char k) {
	switch (k)
	{
	case 'R':
		actorMode_ = RIGIDBODY;
		break;
	case 'P':
		actorMode_ = PARTICLES;
		break;
	default:
		break;
	}
	resetScene();
}

void ActorSystem::resetScene() {
	clearActors();
	clearForces();
}

void ActorSystem::clearActors() {
	while (!actors_.empty()) {
		forceRegistry_->deleteActorRegistry(actors_.front());
		delete actors_.front();
		actors_.pop_front();
	}
	actorCount_ = 0;
}

void ActorSystem::clearForces() {
	if (!forceGen_.empty()) {
		for (auto it = forceGen_.begin(); it != forceGen_.end(); it++) {
			delete* it;
			forceRegistry_->deleteForceRegistry(*it);
		}
		forceGen_.clear();
	}
}

void ActorSystem::setActorMode(ActorMode aMode) {
	actorMode_ = aMode;
}

void ActorSystem::setGenMode(GenerateActorMode genMode) {
	genMode_ = genMode;
}

void ActorSystem::setLimit(int l) {
	actorLimit_ = l;
}

void ActorSystem::generateSpringAnchoredDemo() {
	if (actorMode_ == PARTICLES) {
		Particle* p1 = new Particle(Vector3(0, 45, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 2.0f, 100, 1.0, SPHERE);
		p1->setColor({ 1.0,0.0,0.0,1.0 });
		p1->setSemiEuler();

		Particle* p2 = new Particle(Vector3(5, 45, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 2.0f, 100, 1.0, SPHERE);
		p2->setColor({ 0.0,1.0,0.0,1.0 });
		p2->setSemiEuler();

		Particle* p3 = new Particle(Vector3(-5, 45, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 2.0f, 100, 1.0, SPHERE);
		p3->setColor({ 0.0,1.0,1.0,1.0 });
		p3->setSemiEuler();

		AnchoredSpringFG* fAnchor = new AnchoredSpringFG(5, 20, Vector3(0, 50, 0));

		forceRegistry_->addRegistry(fAnchor, p1);
		forceRegistry_->addRegistry(fAnchor, p2);
		forceRegistry_->addRegistry(fAnchor, p3);
		forceGen_.push_back(fAnchor);
		actors_.push_back(p1);
		actors_.push_back(p2);
		actors_.push_back(p3);
	}
	else {
		RigidBody* rb1 = new RigidBody(gPhysics_, gScene_,
			CreateShape(PxSphereGeometry(2)), Vector3(0, 45, 0), 1, 1, Vector4(1, 0, 0, 1));
		RigidBody* rb2 = new RigidBody(gPhysics_, gScene_,
			CreateShape(PxSphereGeometry(2)), Vector3(5, 45, 0), 1, 1, Vector4(0, 1, 0, 1));
		RigidBody* rb3 = new RigidBody(gPhysics_, gScene_,
			CreateShape(PxSphereGeometry(2)), Vector3(-5, 45, 0), 1, 1, Vector4(0, 1, 1, 1));

		AnchoredSpringFG* fAnchor = new AnchoredSpringFG(100, 20, Vector3(0, 50, 0));

		forceRegistry_->addRegistry(fAnchor, rb1);
		forceRegistry_->addRegistry(fAnchor, rb2);
		forceRegistry_->addRegistry(fAnchor, rb3);
		forceGen_.push_back(fAnchor);
		actors_.push_back(rb1);
		actors_.push_back(rb2);
		actors_.push_back(rb3);

		rb1->disableGravity();
		rb2->disableGravity();
		rb3->disableGravity();
	}
}

void ActorSystem::generateSpringDemo(bool elastic) {
	if (actorMode_ == PARTICLES) {
		//Dos particulas unidas mediente un muelle
		Particle* p1 = new Particle(Vector3(20, 10, 0), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);
		Particle* p2 = new Particle(Vector3(0, 10, 0), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);
		Particle* p3 = new Particle(Vector3(0, 50, -5), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);
		Particle* p4 = new Particle(Vector3(0, 0, -5), Vector3(0), Vector3(0), 2.0f, 100, 1.0, SPHERE);

		p1->setColor({ 0.0,0.0,1.0,1.0 });
		p2->setColor({ 0.0,1.0,0.0,1.0 });
		p3->setColor({ 1.0,0.0,0.0,1.0 });
		p4->setColor({ 1.0,1.0,0.0,1.0 });

		p2->setMass(2.0);

		SpringForceGenerator* sfg1 = new SpringForceGenerator(Vector3(0, 10, 0), 1, 10, p2, elastic);
		SpringForceGenerator* sfg2 = new SpringForceGenerator(Vector3(0, 10, 0), 1, 10, p1, elastic);
		SpringForceGenerator* sfg3 = new SpringForceGenerator(Vector3(0, 25, 0), 1, 30, p4, elastic);
		SpringForceGenerator* sfg4 = new SpringForceGenerator(Vector3(0, 25, 0), 1, 30, p3, elastic);

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

		actors_.push_back(p1);
		actors_.push_back(p2);
		actors_.push_back(p3);
		actors_.push_back(p4);
	}
	else {
		RigidBody* rb1 = new RigidBody(gPhysics_, gScene_,
			CreateShape(PxSphereGeometry(2)), Vector3(20, 10, 0), 1, 1, Vector4(0, 0, 1, 1));
		RigidBody* rb2 = new RigidBody(gPhysics_, gScene_,
			CreateShape(PxSphereGeometry(2)), Vector3(0, 10, 0), 1, 1, Vector4(0, 1, 0, 1));
		RigidBody* rb3 = new RigidBody(gPhysics_, gScene_,
			CreateShape(PxSphereGeometry(2)), Vector3(0, 50, -5), 1, 1, Vector4(1, 0, 0, 1));
		RigidBody* rb4 = new RigidBody(gPhysics_, gScene_,
			CreateShape(PxSphereGeometry(2)), Vector3(0, 0, -5), 1, 1, Vector4(1, 1, 0, 1));


		SpringForceGenerator* sfg1 = new SpringForceGenerator(Vector3(0, 10, 0), 100, 10, rb2, elastic);
		SpringForceGenerator* sfg2 = new SpringForceGenerator(Vector3(0, 10, 0), 100, 10, rb1, elastic);
		SpringForceGenerator* sfg3 = new SpringForceGenerator(Vector3(0, 25, 0), 100, 30, rb4, elastic);
		SpringForceGenerator* sfg4 = new SpringForceGenerator(Vector3(0, 25, 0), 100, 30, rb3, elastic);

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

		actors_.push_back(rb1);
		actors_.push_back(rb2);
		actors_.push_back(rb3);
		actors_.push_back(rb4);

		rb1->disableGravity();
		rb2->disableGravity();
		rb3->disableGravity();
		rb4->disableGravity();
	}
}

void ActorSystem::generateBuoyancyDemo() {
	if (actorMode_ == PARTICLES) {
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

		bfg_ = new BuoyancyForceGenerator({ 0, 0 ,0 }, 0.0, 1.0, 1000);

		forceGen_.push_back(bfg_);
		actors_.push_back(p1);
		actors_.push_back(p2);
		actors_.push_back(p3);

		forceRegistry_->addRegistry(bfg_, p1);
		forceRegistry_->addRegistry(gF_, p1);

		forceRegistry_->addRegistry(bfg_, p2);
		forceRegistry_->addRegistry(gF_, p2);

		forceRegistry_->addRegistry(bfg_, p3);
		forceRegistry_->addRegistry(gF_, p3);
	}
	else {
		RigidBody* rb1 = new RigidBody(gPhysics_, gScene_,
			CreateShape(PxBoxGeometry(3, 3, 3)), Vector3(0, 20, 0), 1, 1, Vector4(1, 1, 0, 1));
		rb1->setMass(500);
		RigidBody* rb2 = new RigidBody(gPhysics_, gScene_,
			CreateShape(PxBoxGeometry(3, 3, 3)), Vector3(0, 20, -10), 1, 1, Vector4(1, 0, 0, 1));
		rb2->setMass(100);
		RigidBody* rb3 = new RigidBody(gPhysics_, gScene_,
			CreateShape(PxBoxGeometry(3, 3, 3)), Vector3(0, 20, 10), 1, 1, Vector4(1, 0, 1, 1));
		rb3->setMass(1000);

		bfg_ = new BuoyancyForceGenerator({ 0, 0 ,0 }, 0.0, 1.0, 1000);

		forceGen_.push_back(bfg_);
		actors_.push_back(rb1);
		actors_.push_back(rb2);
		actors_.push_back(rb3);

		forceRegistry_->addRegistry(bfg_, rb1);
		//forceRegistry_->addRegistry(gF_, p1);

		forceRegistry_->addRegistry(bfg_, rb2);
		//forceRegistry_->addRegistry(gF_, p2);

		forceRegistry_->addRegistry(bfg_, rb3);
		//forceRegistry_->addRegistry(gF_, p3);
	}
}

void ActorSystem::changeK(char k){
	if (genMode_ == SPRING_MODE_A) {
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

void ActorSystem::changeVolume(char v){
	if (bfg_ != nullptr && genMode_ == SPRING_MODE_A) {
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

void ActorSystem::changeMass(char m) {
	for (auto it = actors_.begin(); it != actors_.end();) {
		int sign = m == '0' ? 1 : -1;
		Actor* a = (*it);
		if (a->getMass() + sign > 0)
			a->setMass(a->getMass() + sign);

		cout << "m: " << a->getMass();
		m == '0' ? cout << " (+)\n" : cout << " (-)\n";

		it++;
	}
}
