/*
 * Sparkle.hpp
* See Sparkle.cpp
 */

#ifndef GAME_SPARKLE_HPP
#define GAME_SPARKLE_HPP

#include "../../common/Types.hpp"
#include "Particle.hpp"

GAME_NAMESPACE_BEGIN

class Sparkle  {
public:
	Sparkle();
	virtual ~Sparkle();
	void update();
};

GAME_NAMESPACE_END
#endif
