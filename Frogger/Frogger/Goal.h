#pragma once

#include "../Particle.h"
#include "../ActorSystem.h"

class Goal : public Particle {
protected:
	bool isActive_; //Booleano que indica si se ha llegado a esta meta o no
	double size_ = 35.0;
	
	ActorSystem* confettiGenerator_;
	int heightGen_ = 100;

public:
	Goal(Vector3 pos);
	virtual ~Goal();
	void update(double t);

	void createConffetiExplosion();

	void setActive(bool a) { isActive_ = a; };
	inline bool getActive() { return isActive_; };
	void setGoal();
	bool insideGoal(Vector3 pos);



};

