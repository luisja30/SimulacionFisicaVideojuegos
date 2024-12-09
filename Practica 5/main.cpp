#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Vector3D.h"
#include "Particle.h"
#include "Projectile.h"
#include "RainGenerator.h"
#include "HoseGenerator.h"
#include "MistGenerator.h"
#include "RigidBody.h"
#include "RigidSystem.h"
#include "ActorSystem.h"

#include <iostream>

std::string display_text = "";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;


RenderItem* renderItemCenter = NULL;
RenderItem* renderItemX = NULL;
RenderItem* renderItemY = NULL;
RenderItem* renderItemZ = NULL;

PxTransform transformCenter;
PxTransform transformAxisX;
PxTransform transformAxisY;
PxTransform transformAxisZ;

Particle* particula_ = NULL;
Projectile* proyectil_ = NULL;

std::vector<Particle*> particles_;

ParticleGenerator* particleSys_ = NULL;
RigidBody* rb_ = NULL;
RigidSystem* rigidSys_ = NULL;
ActorSystem* actorSys_ = NULL;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	//particleSys_ = new ParticleGenerator(Vector3(0, 0, 0), 1000);
	//particleSys_->setMode(5);

	//rb_ = new RigidBody(gPhysics, gScene, CreateShape(PxBoxGeometry(10, 5, 6)), Vector3(0, 30, 0), 5.0, 1.0, Vector4(0, 0, 1, 1));
	//rb_->setAngularVelocity(Vector3(0, 5, 0));

	//Suelo
	PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({0,-300,0}));
	PxShape* shape = CreateShape(PxBoxGeometry(1000, 0.1, 1000));
	suelo->attachShape(*shape);
	gScene->addActor(*suelo);
	RenderItem* item;
	item = new RenderItem(shape, suelo, { 0,1,1,1 });

	//rigidSys_ = new RigidSystem(gPhysics, gScene, Vector3(0), 1000);
	actorSys_ = new ActorSystem(gPhysics, gScene, Vector3(0), 1000);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
	gScene->simulate(t);
	gScene->fetchResults(true);
	//particula_->integrate(t);
	/*if(proyectil_ != nullptr) 
		proyectil_->integrate(t);*/

	//Lista de particulas (proyectiles)
	for (int i = 0; i < particles_.size(); i++) {
		if (particles_[i] != nullptr) {
			particles_[i]->integrate(t);
			if (particles_[i]->isGrounded()) {
				delete particles_[i];
				particles_[i] = nullptr;
			}
		}
	}

	//particleSys_->update(t);
	//rigidSys_->update(t);
	actorSys_->update(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//DeregisterRenderItem(renderItemCenter);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key)) {
	//case ' ':	break;
	case 'B': {
		particles_.push_back(new Projectile(Vector3(camera.p), Vector3(-100, 0, -100), Vector3(0, 0, 0)));
		break;
	}
	/*case 'Z': {
		particleSys_->setMode(0);
		break;
	}
	case 'X': {
		particleSys_->setMode(1);
		break;
	}
	case 'C': {
		particleSys_->setMode(2);
		break;
	}
	case 'V': {
		particleSys_->setMode(3);
		break;
	}
	case 'N': {
		particleSys_->setMode(4);
		break;
	}
	case '5': {
		particleSys_->setMode(5);
		break;
	}
	case '6': {
		particleSys_->setMode(6);
		break;
	}
	case '7': {
		particleSys_->setMode(7);
		break;
	}
	case '8': {
		particleSys_->setMode(8);
		break;
	}
	case '-': {
		particleSys_->changeK('-');
		break;
	}
	case '+': {
		particleSys_->changeK('+');
		break;
	}
	case '.': {
		particleSys_->changeVolume('.');
		break;
	}
	case ',': {
		particleSys_->changeVolume(',');
		break;
	}
	case '0': {
		particleSys_->changeMass('0');
		break;
	}
	case '9': {
		particleSys_->changeMass('9');
		break;
	}*/

	default:
		break;
	}

	actorSys_->keyPressed(key);
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}