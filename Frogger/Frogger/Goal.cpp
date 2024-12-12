#include "Goal.h"

Goal::Goal(Vector3 pos)
	: Particle(pos, Vector3(0), Vector3(0), 35.0, 1000, 1.0, SPHERE), 
	isActive_(true) {
	setColor(Vector4(0, 1, 0, 1));
}
