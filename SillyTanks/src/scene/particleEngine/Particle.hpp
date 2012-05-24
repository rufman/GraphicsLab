/*
 * particle.hpp
*Particle is a base class for the creation of a real particle.
 */

#ifndef GAME_PARTICLE_HPP
#define GAME_PARTICLE_HPP

//std includes
#include <string>
#include "../../common/Types.hpp"
namespace game_space {


class Particle {
public:
	virtual ~Particle(){}
	virtual void resetParticle(Point startPosition,Vector3D startAcceleration) = 0;
	virtual void update(float seconds) = 0;

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
	float size;

	std::string _particleTexturePath;
};

}
#endif /* PARTICLE_HPP_ */
