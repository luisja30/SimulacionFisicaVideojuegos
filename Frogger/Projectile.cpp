#include "Projectile.h"

//Projectile::Projectile() : Particle(){
//}

Projectile::Projectile(Vector3 pos, Vector3 vel, Vector3 acel) : Particle(pos, vel, acel) {
}

bool Projectile::integrate(double t) {
	tr_.p = tr_.p + vel_ * t;
	vel_ = vel_ - g_ * t;

	return true;
}


