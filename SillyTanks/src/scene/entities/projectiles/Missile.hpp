/*
 * Missile.hpp
 *
 *  Created on: 16.05.2012
 *      Author: gregory
 */

#ifndef MISSILE_HPP_
#define MISSILE_HPP_

// Base class include
#include "../collisiondetection/Projectile.hpp"

// common includes
#include "../../../common/Types.hpp"

//particle engine includes
#include "../../particleEngine/ParticleEngine.hpp"
#include "../../particleEngine/Smoke.hpp"
#include "../../particleEngine/Explosion.hpp"

namespace game_space {

class Missile : public Projectile
{
public:
	/**
	 * Constructor
	 *
	 * @param[in, out]	scene		Owning scene
	 * @param[in]		size		Size of the bullet
	 */
	Missile( Scene &scene, float size = 0.4f );

	/** Destructor */
	~Missile();

	/** Draw the object to the screen. */
	void draw() const;

	// Get/Set functions
	float getSize() const {return _size;}

	const Vector3D &getVelocity() const {return _velocity;}
	void setVelocity( const Vector3D &velocity );

	void setTargetPosition(const Point targetPosition);
	bool isDetonated();

	/**
	 * Update bullet data by moving it for 'seconds' seconds
	 *
	 * @param[in]	seconds		Seconds to move the bullet object
	 */
	void move( float seconds );

private:
	/** Size of the bullet */
	float _size;

	/** Velocity of the bullet */
	Vector3D _velocity;
	Vector3D _toTarget;
	Point _targetPosition;

	int _waitBeforeAimingCounter;
	ParticleEngine<Smoke> *_missileSmokeParticleEngine;
	ParticleEngine<Explosion>* _missileExplosionParticleEngine;
	bool _didDamage;

}; // class Bullet

}


#endif /* MISSILE_HPP_ */
