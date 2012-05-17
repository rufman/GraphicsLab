/*
 * Missile.hpp
 *
 *  Created on: 16.05.2012
 *      Author: gregory
 */

#ifndef MISSILE_HPP_
#define MISSILE_HPP_

// Base class include
#include "../../common/Drawable.hpp"

// Includes
#include "../../common/Types.hpp"

namespace game_space {

class Missile : public Drawable
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

	const Point &getPosition() const {return _position;}
	void setPosition( const Point &position );

	const Vector3D &getVelocity() const {return _velocity;}
	void setVelocity( const Vector3D &velocity );

	/**
	 * Update bullet data by moving it for 'seconds' seconds
	 *
	 * @param[in]	seconds		Seconds to move the bullet object
	 */
	void move( float seconds );

private:
	/** Size of the bullet */
	float _size;

	/** Position of the bullet compared to the cannon */
	Point _position;

	/** Velocity of the bullet */
	Vector3D _velocity;

	int _counter;

}; // class Bullet

}


#endif /* MISSILE_HPP_ */
