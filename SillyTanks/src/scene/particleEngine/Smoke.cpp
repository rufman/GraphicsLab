/*
 * Smoke.cpp
 * This class creates a smoke particle that slowly disappears.
 */

//header include
#include "Smoke.hpp"

namespace game_space {

Smoke::Smoke() {
	_particleTexturePath = SMOKE_TEXTURE;
}

Smoke::~Smoke() {
}

void Smoke::resetParticle(Point startPosition,Vector3D startAcceleration)
{
	timeToLive = 1.0f; // Give it new life
	fade = float(rand() % 100) / 200.0f + 0.4f;// Random fade value
	x = startPosition.x+rand()%3-1.5;
	y = startPosition.y+rand()%3-1.5;
	z = startPosition.z+rand()%3-1.5;
	vx = rand()%3-1.5;// X velocity
	vy = rand()%3-1.5;// Y velocity
	vz = rand()%3-1.5;// Z velocity
	ax = startAcceleration.x;
	ay = startAcceleration.y;
	az = startAcceleration.z;
	r = 0;
	g = 1.0;
	b = 0;
}

void Smoke::update(float seconds) {

		// If the particle is active
		if (active) {

			x += vx*seconds;
			y += vy*seconds;// + 0.5*GRAVITATIONAL_ACCELERATION*seconds*seconds;
			z += vz*seconds;

			//vy += GRAVITATIONAL_ACCELERATION*seconds;

			timeToLive -= fade * seconds; // Reduce particles life by 'fade'
		}
}

}
