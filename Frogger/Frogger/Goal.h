#pragma once

#include "../Particle.h"

class Goal : public Particle {
protected:
	bool isActive_; //Booleano que indica si se ha llegado a esta meta o no
	double size_ = 35.0;
public:
	Goal(Vector3 pos);
	virtual ~Goal() {};
	void setActive(bool a) { isActive_ = a; };
	inline bool getActive() { return isActive_; };
	bool insideGoal(Vector3 pos) {
		return pos.x < this->getPosition().x + size_
			&& pos.x > this->getPosition().x - size_
			&& pos.z < this->getPosition().z + size_
			&& pos.z > this->getPosition().z - size_;
	}
};

