/*
 * Sparkle.hpp
* See Sparkle.cpp
 */

#ifndef GAME_SPARKLE_HPP
#define GAME_SPARKLE_HPP

//base class include
#include "Particle.hpp"

//common includes
#include "../../common/Types.hpp"

namespace game_space {

class Sparkle  {
public:
	Sparkle();
	virtual ~Sparkle();
	void update();
};

}
#endif
