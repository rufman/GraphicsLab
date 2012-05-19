/*
 * target.hpp
*target is a base class for all the entities that can be targeted by another target..
 */
#ifndef GAME_TARGET_HPP
#define GAME_TARGET_HPP

namespace game_space {
class Projectile;

class Target {
public:
	virtual bool isHit(Projectile projectile);
	virtual bool collidesWithOtherTarget(Target target);
};

}
#endif
