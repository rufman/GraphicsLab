/*
 * CollisionBox.hpp
 *
 *  Created on: 21.05.2012
 *      Author: gmazlami
 */

#ifndef COLLISIONBOX_HPP_
#define COLLISIONBOX_HPP_

#include "Types.hpp"
#include "Utils.hpp"


namespace game_space {

class CollisionBox {

public:

	void setMax(float x, float y, float z);

	void setMin(float x, float y, float z);


	Point getMin();

	Point getMax();


private:

	Point min;
	Point max;


};

} /* namespace game_space */
#endif /* COLLISIONBOX_HPP_ */
