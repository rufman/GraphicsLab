/*
 * CollisionBox.cpp
 *
 *  Created on: 21.05.2012
 *      Author: gmazlami
 */

#include "CollisionBox.hpp"

namespace game_space {

void CollisionBox::setMax(float x_, float y_, float z_){

	Point newMax = Point(x_, y_, z_, 0);

	this->max = newMax;
}

void CollisionBox::setMin(float x_, float y_, float z_){

	Point newMin = Point(x_, y_, z_, 0);

	this->min = newMin;

}

Point CollisionBox::getMax(){
	return this->max;
}

Point CollisionBox::getMin(){
	return this->min;
}





} /* namespace game_space */
