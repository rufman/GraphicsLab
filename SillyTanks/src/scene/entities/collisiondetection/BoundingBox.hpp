/*
 * BoundingBox.hpp
 *
 */

#ifndef BOUNDINGBOX_HPP_
#define BOUNDINGBOX_HPP_

namespace game_space {

class BoundingBox{
public:
	float _x,_y,_z;
public:
	BoundingBox(float x,float y,float z);
	~BoundingBox();

};
}
#endif
