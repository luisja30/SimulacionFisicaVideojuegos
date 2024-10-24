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

#include <iostream>

std::string display_text = "This is a test";


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
RainGenerator* rainGen_ = NULL;
HoseGenerator* hoseGen_ = NULL;

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


	//Practica 0
	/*Vector3D ejeX(10, 0, 0), ejeY(0, 10, 0), ejeZ(0, 0, 10);

	transformAxisX = PxTransform(ejeX.getX(), ejeX.getY(), ejeX.getZ());
	transformAxisY = PxTransform(ejeY.getX(), ejeY.getY(), ejeY.getZ());
	transformAxisZ = PxTransform(ejeZ.getX(), ejeZ.getY(), ejeZ.getZ());
	transformCenter = PxTransform(0, 0, 0);*/

	//renderItemCenter = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &transformCenter, { 1.0,1.0,1.0,1.0 });
	//renderItemX = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &transformAxisX, { 1.0,0.0,0.0,1.0 });
	//renderItemY = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &transformAxisY, { 0.0,1.0,0.0,1.0 });
	//renderItemZ = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &transformAxisZ, { 0.0,0.0,1.0,1.0 });


	//Practica 1
	//particula_ = new Particle(Vector3(0, 0, 0), Vector3(10, 0, 0), Vector3(20, 0, 0));
	//particula_->setColor({ 1.0,0.0,0.0,1.0 });

	//Practica 2
	//particleSys_ = new ParticleGenerator(Vector3(0, 70, -90), Vector3(0, 10, 0), Vector3(0, 1, 0), 300);
	//rainGen_ = new RainGenerator(Vector3(0, 150, 0), 1000, 100);
	hoseGen_ = new HoseGenerator(Vector3(-30, 150, -300), 1000);
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

	//Lista de particulas
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
	//rainGen_->update(t);
	hoseGen_->update(t);
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

	switch(toupper(key))
	{
	case 'B': {
		particles_.push_back(new Projectile(Vector3(camera.p), Vector3(-100, 0, -100), Vector3(0, 0, 0)));
		break;
	}
	//case ' ':	break;
	case ' ': {
		break;
	}
	default:
		break;
	}
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