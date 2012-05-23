/*
 * Projectile.hpp
 */

#ifndef PROJECTILE_HPP_
#define PROJECTILE_HPP_

#include "../../../common/Drawable.hpp"

#include <vector>

namespace game_space {

class Projectile:public Drawable {

public:
	enum ProjectileType
	{
		BULLET,
		MISSILE
	}_projectileType;
	//this is necessary for the AI to know what tank took a shot on it.
	int _projectileOwnerID;


public:
	Projectile(Scene& scene,ProjectileType projectileType,int projectileOwnerId);

	//a projectile has a bounding radius to first check if the projectile is even near to the target
	//(check if distance between projectile and target is higher than bounding radiuses added up)
	float getBoundingRadius() const;
	Point getPosition();

	/** Draw the object to the screen. */
	void draw() const;

private:

	Point _position;
};

}

#endif
