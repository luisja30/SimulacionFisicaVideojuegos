#include "AnchoredSpringFG.h"

AnchoredSpringFG::AnchoredSpringFG(double k, double resting_lenght, const Vector3& anchor_pos)
	:SpringForceGenerator(anchor_pos, k, resting_lenght, nullptr) {
	other_ = new Particle(anchor_pos, { 0,0,0 }, { 0,0,0 }, 2.0, 10000, 0.0, BOX);
}

AnchoredSpringFG::~AnchoredSpringFG() {
	delete other_;
}
