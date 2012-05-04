/*
 * Smoke.cpp
 * This class creates a smoke particle that slowly disappears.
 */

//header include
#include "Smoke.hpp"

GAME_NAMESPACE_BEGIN

Smoke::Smoke() {
	_particleTexturePath = SMOKE_TEXTURE;
}

Smoke::~Smoke() {
}

void Smoke::resetParticle(Point startPosition,Vector3D startAcceleration)
{
	active = true;
	timeToLive = 1.0f; // Give it new life
	fade = float(rand() % 100) / 1000.0f + 0.33f;// Random fade value
	x = startPosition.x;
	y = startPosition.y;
	z = startPosition.z;
	vx = rand()%10-5;// X velocity
	vy = rand()%10-5;// Y velocity
	vz = rand()%10-5;// Z velocity
	ax = startAcceleration.x;
	ay = startAcceleration.y;
	az = startAcceleration.z;
	r = 0;
	g = 1.0;
	b = 0;
}

void Smoke::update(float seconds,Point startPosition,Vector3D startAcceleration) {

		// If the particle is active
		if (active) {

			x += vx*seconds;
			y += vy*seconds + 0.5*GRAVITATIONAL_ACCELERATION*seconds*seconds;
			z += vz*seconds;

			vy += GRAVITATIONAL_ACCELERATION*seconds;

			timeToLive -= fade * seconds; // Reduce particles life by 'fade'

			// If particle is burned out
			if (timeToLive < 0.0f) {
				resetParticle(startPosition,startAcceleration);
			}
		}
}

GAME_NAMESPACE_END
