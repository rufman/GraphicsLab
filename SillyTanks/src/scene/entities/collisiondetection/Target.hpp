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

class Point;

class Target:public Drawable {
public:
	enum TargetType
	{
		TANK,
		TOWER
	}_targetType;
private:
	std::vector<BoundingBox> _boundingBoxes;
	BoundingBox* _boundingBox;

protected:
	Point _position;
public:
	Target(Scene &scene,TargetType type);
	bool checkHit(Projectile* projectile_);
	virtual bool collidesWithOtherTarget(Target* target);

	//a projectile has a bounding radius to first check if the projectile is even near to the target
	//(check if distance between projectile and target is higher than bounding radiuses added up)
	float getBoundingRadius() const;

	void addBoundingBox(BoundingBox box);
		std::vector<BoundingBox> getBoundingBoxes() const;
	/** Draw the object to the screen. */

		//TODO: I don't think it is necessary to have multiple boundinboxes.
		//we don't gain efficiency or something for it an we would spent time
		//which we could spend for other things


	void draw() const;

	void update(float seconds);

	void reset();

	void setPosition(Point position);
	Point getPosition() const;
};

}
#endif
