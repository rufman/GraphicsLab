/*
 * Explosion.hpp
 */

#ifndef GAME_EXPLOSION_HPP
#define GAME_EXPLOSION_HPP

//base class include
#include "Particle.hpp"

namespace game_space {

class Point;
class Vector3D;

class Explosion:virtual public Particle {
public:
	Explosion();
	virtual ~Explosion();
	virtual void resetParticle(Point startPosition,Vector3D startAcceleration);
	virtual void update(float seconds);
};

}
#endif
