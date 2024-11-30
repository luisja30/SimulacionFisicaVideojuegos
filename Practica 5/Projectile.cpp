#include "Projectile.h"

//Projectile::Projectile() : Particle(){
//}

Projectile::Projectile(Vector3 pos, Vector3 vel, Vector3 acel) : Particle(pos, vel, acel) {
}

void Projectile::integrate(double t) {
	pose_.p = pose_.p + vel_ * t;
	vel_ = vel_ - g_ * t;
}


