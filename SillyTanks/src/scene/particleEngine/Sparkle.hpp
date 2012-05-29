#ifndef GAME_SPARKLE_HPP
#define GAME_SPARKLE_HPP

//base class include
#include "Particle.hpp"

namespace game_space {

class Point;
class Vector3D;

class Sparkle:virtual public Particle {
public:
	Sparkle();
	virtual ~Sparkle();
	virtual void resetParticle(Point startPosition,Vector3D startAcceleration);
	virtual void update(float seconds);
};

}
#endif
