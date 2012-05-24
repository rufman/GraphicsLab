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

class TowerAI {
public:
	enum TOWERAI_STRATEGY {
		EXPLORE, HUNT
	} _strategy;

	std::vector<Message*>* _aiMessages;
	Tower* _tower;
	Target* _currentTarget;
	std::vector<Point>* _path;
	Scene& _scene;

	//delay the shooting a bit
	float reloadTime;
public:
	TowerAI(Scene &scene, std::vector<Message*>* aiMessages);
	virtual ~TowerAI();

	//AI methods
	void brainTick(float seconds);

	//if the conditions are met, the ai switches the strategy
	void switchStrategy(TOWERAI_STRATEGY strategy, Target* target);

	//picks the target that sense has found from the messages or returns -1 if no target has been found.
	Target* pickTarget();

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
