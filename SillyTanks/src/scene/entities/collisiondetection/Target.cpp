/*
 * Target.cpp
 */

#include "Target.hpp"

namespace game_space {
Target::Target(Scene &scene,TargetType type):Drawable(scene),_targetType(type),_position(Point(0,0,0)){}

bool Target::collidesWithOtherTarget(Target* target)
{
	return false;
}

BoundingBox* Target::getBoundingBox(){
	return _boundingBox;

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

float Target::getBoundingRadius(){
	return _boundingRadius;
}


}




