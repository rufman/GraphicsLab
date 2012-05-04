/*
 * particle.hpp
* See particle.cpp
 */

#ifndef GAME_PARTICLE_HPP
#define GAME_PARTICLE_HPP

//common includes
#include "../../common/Types.hpp"
#include "../../common/TGATexture.hpp"

GAME_NAMESPACE_BEGIN

class Particle {
public:
	virtual void resetParticle(Point startPosition,Vector3D startAcceleration) = 0;
	virtual void update(float seconds,Point startPosition,Vector3D startAcceleration) = 0;

public:

	bool active; // Active (Yes/No)
	float timeToLive; // Particle Life
	float fade; // Fade Speed
	float r; // Red Value
	float g; // Green Value
	float b; // Blue Value
	float x; // X Position
	float y; // Y Position
	float z; // Z Position
	float vx; // X Velocity
	float vy; // Y Velocity
	float vz; // Z Velocity
	float ax; // X Acceleration
	float ay; // Y Acceleration
	float az; // Z Acceleration

	std::string _particleTexturePath;
};

GAME_NAMESPACE_END
#endif /* PARTICLE_HPP_ */
