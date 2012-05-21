/*
 * SmallTower.hpp
 */

#ifndef SMALLTOWER_H_
#define SMALLTOWER_H_

#include "Tower.hpp"
#include "../../../common/TGATexture.hpp"

namespace game_space {

class SmallTower: public game_space::Tower {
public:
	SmallTower(Scene &scene,bool isAIControlled);
		virtual ~SmallTower();
		void draw() const;

	private:
		TGATexture* _tankTexture;
};

} /* namespace game_space */
#endif /* SMALLTOWER_H_ */