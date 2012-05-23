#ifndef GAME_SMOKE_HPP
#define GAME_SMOKE_HPP

//base class include
#include "Particle.hpp"

namespace game_space {

class Point;
class Vector3D;

class Smoke:virtual public Particle {
public:
	Smoke();
	virtual ~Smoke();
	virtual void resetParticle(Point startPosition,Vector3D startAcceleration);
	virtual void update(float seconds);
};

}
#endif
