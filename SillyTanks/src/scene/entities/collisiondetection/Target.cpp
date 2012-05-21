/*
 * Target.cpp
 *
 *  Created on: May 21, 2012
 *      Author: leviathan
 */

#include "Target.hpp"

namespace game_space {
Target::Target(Scene &scene,TargetType type):Drawable(scene),_targetType(type){}
bool Target::isHit(Projectile projectile){
	return false;
}
bool Target::collidesWithOtherTarget(Target target)
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
}




