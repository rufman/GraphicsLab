/*
 * TankAI.hpp
 *
 */

#ifndef TANKAI_H_
#define TANKAI_H_

#include "../../common/Definitions.hpp"
#include "Message.hpp"
#include <vector>
#include "../entities/targets/Tank.hpp"
#include "../Scene.hpp"

namespace game_space {

class MessageSubBus;

class TankAI {
public:
	enum TANKAI_STRATEGY
	{
		EXPLORE,
		HUNT,
		ESCAPE
	}_strategy;

	Scene& _scene;

	MessageSubBus* _aiMessages;
	Tank* _tank;
	Target* _currentTarget;
	std::vector<Point>* _path;

public:
	TankAI(Scene &scene,MessageSubBus* aiMessages);
	virtual ~TankAI();
	//ai methods
	//main method of the ai, activates all the other ai methods, represents one single reflection
	void brainTick(float seconds);

	//if the conditions are met, the ai switches the strategy
	void switchStrategy(TANKAI_STRATEGY strategy,Target* target);

	//sense processes the messages sent to the ai, that means for instance when you hear something, the ai is allowed to react on this.
	void sense();

	//strategy methods
	//exploring is activated as long as the tank did not see(vision of 180 degrees) or hear another tank.
	void explore();

	//hunt is activated if the tank has a found a target and has enough life energy to win
	void hunt();

	//the tank is damaged or out of special weapons and the hunt takes quite long, so some reinforcement could be of use.
	//void reinforcement();

	//the tank can not flee and tries to defend itself with its entire force (higher precision shots and all the weapons are used)
	//void rampage();

	//the tank has lost a lot of energy or the hunt took a lot of time, the tank flees from its enemy.
	void escape();

	//helper methods
	void followPath();
	void aim();
};

}
#endif /* TANKAI_H_ */
