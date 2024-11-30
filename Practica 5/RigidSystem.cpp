#include "RigidSystem.h"

RigidSystem::RigidSystem(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos) :
	gPhysics_(gPhysics), gScene_(gScene), pos_(pos) {
}

RigidSystem::~RigidSystem() {
	//Recorremos la lista y vamos eliminando las partículas para que no quede basura
	for (RigidBody* r : rigidBodies_)
		delete r;
	rigidBodies_.clear();
}

void RigidSystem::update(double t) {
	if (genMode_ != SPRING_MODE_R && genMode_ != EXPLOSION_R) {
		if (rigidCount_ < rigidLimit_) {
			generateRigid();
		}
	}

	//forceRegistry_->updateForces();

	for (auto it = rigidBodies_.begin(); it != rigidBodies_.end();) {
		if (*it != nullptr) {
			(*it)->update(t);
			//Comprobar rango de recorrido o si la particula sigue viva
			if (!(*it)->getAlive() /*|| ((*it)->getPosition() - pos_).magnitude() > range_ */ ) {
				//forceRegistry_->deleteParticleRegistry(*it);
				delete* it;
				it = rigidBodies_.erase(it);
				rigidCount_--;
			}
			else it++;
		}
	}

}

void RigidSystem::generateRigid() {
	Vector3 newPos = pos_, newVel = { 0,0,0 };
	double radius = 0;
	double newLifeTime = 0;
	RigidBody* newRigid = nullptr;
	double newMass = 1;

	switch (genMode_) {
	case RAIN_R:
		radius = 100;
		//Generamos una posicion aletaria en X y Z
		newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
		newPos.y = pos_.y;
		newPos.z += std::normal_distribution<double>(-radius, radius)(rd);
		//Vector3 newPos = Vector3(posX, posY, posZ);

		//Las particulas se mueven solo en el eje Y
		newVel.y = -300;

		//Añadimos particula
		newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(1)), newPos, 1, 1, Vector4(0, 0, 1, 1));
		newRigid->setVelocity(newVel);
		rigidBodies_.push_back(newRigid);

		//Adherimos una fuerza a la particula
		//forceRegistry_->addRegistry(gF_, newParticle);
		break;

	case MIST_R:
		radius = 50;
		newPos.x = std::normal_distribution<double>(pos_.x - radius, pos_.x + radius)(rd);
		newPos.y = std::normal_distribution<double>(pos_.y - radius, pos_.y + radius)(rd);
		newPos.z = std::normal_distribution<double>(pos_.z - radius, pos_.z + radius)(rd);
		//Vector3 newPos = Vector3(posX, posY, posZ);

		newVel.x = std::uniform_real_distribution<double>(-1, 1)(rd);
		newVel.y = std::uniform_real_distribution<double>(-1, 1)(rd);
		newVel.z = std::uniform_real_distribution<double>(-1, 1)(rd);
		//Vector3 newVel = Vector3(velX, velY, velZ);

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
		//Vector3 newPos = Vector3(posX, posY, posZ);

		newVel.x = std::uniform_real_distribution<double>(-10, 10)(rd);
		newVel.y = std::uniform_real_distribution<double>(-300, -200)(rd);
		newVel.z = std::uniform_real_distribution<double>(-10, 10)(rd);
		//Vector3 newVel = Vector3(velX, velY, velZ);

		//float size = std::uniform_real_distribution<float>(1.0f, 6.0f)(rd);
		//Añadimos particula
		newRigid = new RigidBody(gPhysics_, gScene_, CreateShape(PxSphereGeometry(5)), newPos, 1, 1, Vector4(0, 0, 1, 1));
		newRigid->setVelocity(newVel);
		newRigid->setAliveTime(2);
		rigidBodies_.push_back(newRigid);
		break;

	//case WIND:
	//	radius = 10;
	//	newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
	//	newPos.y = origin_.y;
	//	newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

	//	//Añadimos particula
	//	newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
	//	newParticle->setColor({ 0,0,1,1 });

	//	/*newMass = std::normal_distribution<double>(1.0, 20.0)(rd);
	//	newParticle->setMass(newMass);*/

	//	particles_.push_back(newParticle);

	//	forceRegistry_->addRegistry(windF_, newParticle);
	//	break;

	//case TORNADO:
	//	radius = 10;
	//	newPos.x += std::normal_distribution<double>(-radius, radius)(rd);
	//	newPos.y = origin_.y;
	//	newPos.z += std::normal_distribution<double>(-radius, radius)(rd);

	//	//Las particulas se mueven solo en el eje Y
	//	newVel.y = -50;

	//	//Añadimos particula
	//	newParticle = new Particle(newPos, newVel, Vector3(0, 0, 0), 1, 100, 1.0);
	//	newParticle->setColor({ 0,1,1,1 });
	//	particles_.push_back(newParticle);

	//	forceRegistry_->addRegistry(tornadoF_, newParticle);
	//	break;
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
	//clearForces();
}

void RigidSystem::clearRigidBodies() {
	while (!rigidBodies_.empty()) {
		//forceRegistry_->deleteParticleRegistry(rigidBodies_.front());
		delete rigidBodies_.front();
		rigidBodies_.pop_front();
	}
	rigidCount_ = 0;
}
