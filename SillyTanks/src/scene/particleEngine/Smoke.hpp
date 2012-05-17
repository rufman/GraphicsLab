/*
 * Smoke.hpp
* See Smoke.cpp
 */

#ifndef GAME_SMOKE_HPP
#define GAME_SMOKE_HPP

//base class include
#include "Particle.hpp"

//common includes
#include "../../common/Types.hpp"

namespace game_space {

class Smoke:virtual public Particle {
public:
	Smoke();
	virtual ~Smoke();
	virtual void resetParticle(Point startPosition,Vector3D startAcceleration);
	virtual void update(float seconds,Point startPosition,Vector3D startAcceleration);
};

}
#endif
