/*
 * target.hpp
*target is a base class for all the entities that can be targeted by another target..
 */
#ifndef GAME_TARGET_HPP
#define GAME_TARGET_HPP

#include "../../../common/Drawable.hpp"
#include "BoundingBox.hpp"
#include "Projectile.hpp"
#include <vector>

namespace game_space {

class Target:public Drawable {
public:
	enum TargetType
	{
		TANK,
		TOWER
	}_targetType;
private:
	std::vector<BoundingBox> _boundingBoxes;
public:
	Target(Scene &scene,TargetType type);
	virtual bool isHit(Projectile projectile);
	virtual bool collidesWithOtherTarget(Target target);

	//a projectile has a bounding radius to first check if the projectile is even near to the target
	//(check if distance between projectile and target is higher than bounding radiuses added up)
	float getBoundingRadius() const;

	void addBoundingBox(BoundingBox box);
		std::vector<BoundingBox> getBoundingBoxes() const;
	/** Draw the object to the screen. */
	void draw() const;

	void update(float seconds);

	void reset();
};

}
#endif
