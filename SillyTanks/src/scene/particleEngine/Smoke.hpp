/*
 * Smoke.hpp
* See Smoke.cpp
 */

#ifndef GAME_SMOKE_HPP
#define GAME_SMOKE_HPP

#include "Particle.hpp"

//common includes
#include "../../common/Types.hpp"

GAME_NAMESPACE_BEGIN

class Smoke:virtual public Particle {
public:
	Smoke();
	virtual ~Smoke();
	virtual void resetParticle(Point startPosition,Vector3D startAcceleration);
	virtual void update(float seconds,Point startPosition,Vector3D startAcceleration);
};

GAME_NAMESPACE_END
#endif
