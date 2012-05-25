/*
 * smallTank.hpp
 * See smallTank.cpp
 */

#ifndef GAME_SMALLTANK_HPP
#define GAME_SMALLTANK_HPP

#include "../../../common/Definitions.hpp"
#include "../collisiondetection/BoundingBox.hpp"
#include "Tank.hpp"
#include "../../AI/TankAI.hpp"

namespace game_space {

class SmallTank:public Tank {
public:
	SmallTank(Scene &scene,bool isAIControlled);
	virtual ~SmallTank();
	void draw() const;
	void castShadow(const Point &lightPostion) const;
	void setNeighbors() const;

private:
	TGATexture* _tankTexture;
};

}
#endif
