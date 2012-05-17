/*
 * TowerAI.h
 */

#ifndef TOWERAI_H_
#define TOWERAI_H_

namespace game_space {

class TowerAI {
public:
	enum TOWERAI_STRATEGY {
		EXPLORE, HUNT
	} _strategy;
public:
	TowerAI();
	virtual ~TowerAI();

	//AI methods
	void brainTick();

	//if the conditions are met, the ai switches the strategy
	void switchStrategy(TOWERAI_STRATEGY strategy, int target);

	//picks the target that sense has found from the messages or returns -1 if no target has been found.
	int pickTarget();

	//sense processes the messages sent to the ai, that means for instance when you hear something, the ai is allowed to react on this.
	void sense();

	//explore is here just an animation that makes the tower turret rotate to different positions that makes it look like it is aiming.
	void explore();

	//if a target has been found, the tower aims to it and fights until the tank is down.
	void hunt();

	//helper methods
	void aimAndFire(int target);

};

}
#endif /* TOWERAI_H_ */
