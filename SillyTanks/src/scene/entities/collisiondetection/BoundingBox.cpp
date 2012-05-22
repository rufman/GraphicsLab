/*
 * BoundingBox.cpp
 * This class is a bounding box to detect collisions between a to objects
 */
#include "BoundingBox.hpp"
//#include "../../../common/Types.hpp"



namespace game_space {


BoundingBox::BoundingBox(Point* arr){

	frontRightUnder = arr[0];
	frontLeftUnder = arr[1];
	frontRightUpper = arr[2];
	frontLeftUpper = arr[3];

	rearRightUnder = arr[4];
	rearLeftUnder = arr[5];
	rearRightUpper = arr[6];
	rearLeftUpper = arr[7];


}



}

