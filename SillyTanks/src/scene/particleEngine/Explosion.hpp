/*
 * Explosion.hpp
* See Explosion.cpp
 */

#ifndef GAME_EXPLOSION_HPP
#define GAME_EXPLOSION_HPP

//base class include
#include "Particle.hpp"

//common includes
#include "../../common/Types.hpp"

GAME_NAMESPACE_BEGIN

class Explosion  {
public:
	Explosion();
	virtual ~Explosion();
};

GAME_NAMESPACE_END
#endif
