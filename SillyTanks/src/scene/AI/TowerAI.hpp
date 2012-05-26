/*
 * TowerAI.h
 */

#ifndef TOWERAI_H_
#define TOWERAI_H_

#include <vector>

namespace game_space {
class Target;
class Tower;
class Message;
class Scene;
class Point;
class MessageSubBus;

class TowerAI {
public:
	enum TOWERAI_STRATEGY {
		EXPLORE, HUNT
	} _strategy;

	MessageSubBus* _aiMessages;
	Tower* _tower;
	Target* _currentTarget;
	std::vector<Point>* _path;
	Scene& _scene;

	//delay the shooting a bit
	float reloadTime;
public:
	TowerAI(Scene &scene, MessageSubBus* aiMessages);
	virtual ~TowerAI();

	//AI methods
	void brainTick(float seconds);

	//if the conditions are met, the ai switches the strategy
	void switchStrategy(TOWERAI_STRATEGY strategy, Target* target);

	//sense processes the messages sent to the ai, that means for instance when you hear something, the ai is allowed to react on this.
	void sense();

	//explore is here just an animation that makes the tower turret rotate to different positions that makes it look like it is aiming.
	void explore();

	//if a target has been found, the tower aims to it and fights until the tank is down.
	void hunt();

	//helper methods
	void aimAndFire();

};

}
#endif /* TOWERAI_H_ */
