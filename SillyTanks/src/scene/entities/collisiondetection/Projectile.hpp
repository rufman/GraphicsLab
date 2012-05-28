/*
 * Projectile.hpp
 */

#ifndef PROJECTILE_HPP_
#define PROJECTILE_HPP_

#include "../../../common/Drawable.hpp"

#include <vector>

namespace game_space {

class Target;
class Scene;
class Point;

class Projectile:public Drawable {

public:
	enum ProjectileType
	{
		BULLET,
		MISSILE,
		ROBOT
	}_projectileType;
	//this is necessary for the AI to know what tank took a shot on it.
	Target* _projectileOwner;
	bool _detonated;


public:
	Projectile(Scene& scene,ProjectileType projectileType,Target* projectileOwner);
	virtual ~Projectile();

	//a projectile has a bounding radius to first check if the projectile is even near to the target
	//(check if distance between projectile and target is higher than bounding radiuses added up)
	float getBoundingRadius() const;
	void setPosition(Point position);
	const Point getPosition() const;

	/** Draw the object to the screen. */
	void draw() const;

protected:

	Point _position;
};

}

#endif
