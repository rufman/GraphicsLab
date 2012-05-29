/*
 * Smoke.cpp
 * This class creates a smoke particle that slowly disappears.
 */

//header include
#include "Sparkle.hpp"
#include <math.h>
#include <stdlib.h>
#include "../../common/Types.hpp"
#include "../../common/Definitions.hpp"

namespace game_space {

Sparkle::Sparkle() {
	_particleTexturePath = SPARKLE_TEXTURE;
}

Sparkle::~Sparkle() {
}

void Sparkle::resetParticle(Point startPosition,Vector3D startAcceleration)
{
	timeToLive = 1.0f; // Give it new life
	size = 1;
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

void Sparkle::update(float seconds) {

		// If the particle is active
		if (active) {

			x += vx*seconds;
			y += vy*seconds;
			z += vz*seconds;
			size = 0.5 + sin(size);


			timeToLive -= fade * seconds; // Reduce particles life by 'fade'
		}
}

}
