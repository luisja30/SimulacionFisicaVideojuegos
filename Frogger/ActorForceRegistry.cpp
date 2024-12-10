#include "ActorForceRegistry.h"

ActorForceRegistry::ActorForceRegistry(){
}

ActorForceRegistry::~ActorForceRegistry(){
}

void ActorForceRegistry::addRegistry(ForceGenerator* forceGen, Actor* a){
	registryForces_.insert({ forceGen,a });
}

void ActorForceRegistry::deleteActorRegistry(Actor* a){
	for (auto it = registryForces_.begin(); it != registryForces_.end();) {
		if (it->second == a)
			it = registryForces_.erase(it);
		else it++;
	}
}

void ActorForceRegistry::deleteForceRegistry(ForceGenerator* fg) {
	registryForces_.erase(fg);
}

void ActorForceRegistry::updateForces(){
	for (auto it = registryForces_.begin(); it != registryForces_.end(); it++) {
		it->first->updateForce(it->second);
	}
}
