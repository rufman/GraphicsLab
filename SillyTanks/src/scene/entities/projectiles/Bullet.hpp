/**
 * bullet.hpp
 * See bullet.cpp
 */

#ifndef GAME_BULLET_HPP
#define GAME_BULLET_HPP

// Base class include
#include "../collisiondetection/Projectile.hpp"

// common includes
#include "../../../common/Types.hpp"

namespace game_space {

class Bullet : public Projectile
{
public:
	/**
	 * Constructor
	 *
	 * @param[in, out]	scene		Owning scene
	 * @param[in]		size		Size of the bullet
	 */
	Bullet( Scene &scene, float size = 0.4f );

	/** Destructor */
	~Bullet();

	/** Draw the object to the screen. */
	void draw() const;

	// Get/Set functions
	float getSize() const {return _size;}

	const Vector3D &getVelocity() const {return _velocity;}
	void setVelocity( const Vector3D &velocity );

	/**
	 * Update bullet data by moving it for 'seconds' seconds
	 *
	 * @param[in]	seconds		Seconds to move the bullet object
	 */
	void move( float seconds );
	bool isDetonated();

private:
	/** Size of the bullet */
	float _size;

	/** Velocity of the bullet */
	Vector3D _velocity;


}; // class Bullet

}

#endif // GRAPHICSLAB_BULLET_HPP
