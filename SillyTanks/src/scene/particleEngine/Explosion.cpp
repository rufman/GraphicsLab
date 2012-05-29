/*
 * Smoke.cpp
 * This class creates a flashing explosion
 */

//header include
#include "Explosion.hpp"
#include "../../common/Types.hpp"

namespace game_space {

Explosion::Explosion() {
	_particleTexturePath = EXPLOSION_TEXTURE;
}

Explosion::~Explosion() {
}

void Explosion::resetParticle(Point startPosition,Vector3D startAcceleration)
{
	timeToLive = 1.0f; // Give it new life
	fade = 1.0f;// fade value
	x = startPosition.x;
	y = startPosition.y;
	z = startPosition.z;
	vx = 0;// X velocity
	vy = 0;// Y velocity
	vz = 0;// Z velocity
	ax = startAcceleration.x;
	ay = startAcceleration.y;
	az = startAcceleration.z;
	r = 0;
	g = 1.0;
	b = 0;
	size = 3;
}

void Explosion::update(float seconds) {

		// If the particle is active
		if (active) {
			size += 0.3;
			timeToLive -= fade * seconds; // Reduce particles life by 'fade'
		}
}

}
