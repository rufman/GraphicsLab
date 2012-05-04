/*
 * smallTank.hpp
 * See smallTank.cpp
 */

#ifndef GAME_SMALLTANK_HPP
#define GAME_SMALLTANK_HPP

#include "../../common/Definitions.hpp"
#include "Tank.hpp"

GAME_NAMESPACE_BEGIN

class SmallTank:public Tank {
public:
	SmallTank(Scene &scene);
	virtual ~SmallTank();
	void draw() const;
};

GAME_NAMESPACE_END
#endif
