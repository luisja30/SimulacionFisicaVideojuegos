#pragma once
#include <vector>
#include <map>
#include "Actor.h"
#include "ForceGenerator.h"

using namespace std;

class ActorForceRegistry {
protected:
	multimap<ForceGenerator*, Actor*> registryForces_;
public:
	ActorForceRegistry();
	virtual ~ActorForceRegistry();
	void addRegistry(ForceGenerator* forceGen, Actor* a);
	void deleteActorRegistry(Actor* a);
	void deleteForceRegistry(ForceGenerator* fg);
	void updateForces();


};

