/*
 * BoundingBox.hpp
 *
 */

#ifndef BOUNDINGBOX_HPP_
#define BOUNDINGBOX_HPP_

#include "../../../common/Types.hpp"

namespace game_space {




class BoundingBox{


public:

	BoundingBox(Point* arr);

	//corner points of the bounding box;


	//front face of the bounding box
	Point frontLeftUnder;
	Point frontRightUnder;
	Point frontLeftUpper;
	Point frontRightUpper;

	//rear face of the bounding box
	Point rearLeftUnder;
	Point rearRightUnder;
	Point rearLeftUpper;
	Point rearRightUpper;




};
}
#endif
