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


protected:
	BoundingBox* _boundingBox;
	Point _position;
	float _boundingRadius;


public:
	Target(Scene &scene,TargetType type);
	virtual bool checkHit(Projectile* projectile_);
	virtual bool collidesWithOtherTarget(Target* target);

	//a projectile has a bounding radius to first check if the projectile is even near to the target
	//(check if distance between projectile and target is higher than bounding radiuses added up)
	float getBoundingRadius() ;

	BoundingBox* getBoundingBox();


	void draw() const;

	void update(float seconds);

	void reset();

	void setPosition(Point position);
	Point getPosition() const;

	void setLife(float life);
	float getLife();
	void setShield(float shield);
	float getShield();
	void setQtyMissiles(uint qty);
	uint getQtyMissiles();
	void setQtyRobots(uint qty);
	uint getQtyRobots();
	void doDamage(float damage);

	float _shield;
	float _life;
	float _oldLife;
	uint _amountOfMissiles;
	uint _amountOfRobots;
	float _reloadingTime;
};

}
#endif
