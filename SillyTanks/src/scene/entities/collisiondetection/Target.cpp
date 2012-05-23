/*
 * Target.cpp
 */

#include "Target.hpp"

namespace game_space {
Target::Target(Scene &scene,TargetType type):Drawable(scene),_targetType(type),_position(Point(0,0,0)){}
bool Target::isHit(Projectile* projectile){
	return false;
}
bool Target::collidesWithOtherTarget(Target* target)
{
	return false;
}

void Target::addBoundingBox(BoundingBox box)
{
_boundingBoxes.push_back(box);
}

std::vector<BoundingBox> Target::getBoundingBoxes() const
{
	return _boundingBoxes;
}
void Target::draw() const
{

}
void Target::reset()
{

}

void Target::setPosition(Point position) {
	_position = position;
}

Point Target::getPosition() const {
	return _position;
}
}




