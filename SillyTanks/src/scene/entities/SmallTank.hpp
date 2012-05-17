/*
 * smallTank.hpp
 * See smallTank.cpp
 */

#ifndef GAME_SMALLTANK_HPP
#define GAME_SMALLTANK_HPP

#include "../../common/Definitions.hpp"
#include "Tank.hpp"

namespace game_space {

class SmallTank:public Tank {
public:
	SmallTank(Scene &scene);
	virtual ~SmallTank();
	void draw() const;
};

}
#endif
