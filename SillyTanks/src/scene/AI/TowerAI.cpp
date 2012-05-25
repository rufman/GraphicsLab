/*
 * TowerAI.cpp
 */

#include "TowerAI.hpp"
#include "../../common/Utils.hpp"
#include "../../common/Types.hpp"
#include "../../common/Definitions.hpp"
#include "../Scene.hpp"
#include "../entities/collisiondetection/Target.hpp"
#include "../entities/targets/Tower.hpp"
#include "Message.hpp"
#include "DetonationSoundMessage.hpp"
#include "AttackedByMessage.hpp"
#include <iostream>
#include <vector>

namespace game_space {

TowerAI::TowerAI(Scene &scene, std::vector<Message*>* aiMessages) :
		_strategy(TowerAI::EXPLORE), _aiMessages(aiMessages), _currentTarget(NULL), _scene(scene) {

}

TowerAI::~TowerAI() {

}

void TowerAI::brainTick(float seconds) {

	//TODO: add reloading time
	sense();
	// if pickTarget is null, sensing failed and we have to explore further
	if (_currentTarget == NULL) {
		switchStrategy(EXPLORE, NULL);
	}
	else
	{
		switchStrategy(HUNT,_currentTarget);
	}

	//we have a target and we hunt it
	switch (_strategy) {
	case HUNT: {
		hunt();
		break;
	}
	case EXPLORE: {
		explore();
		break;
	}
	}

}

void TowerAI::switchStrategy(enum TOWERAI_STRATEGY newStrategy, Target* target) {
	_strategy = newStrategy;
	_currentTarget = target;
}

void TowerAI::sense() {
	for (std::vector<Message*>::iterator messageIterator = _aiMessages->begin(); messageIterator != _aiMessages->end(); messageIterator++) {
		Message* message = *messageIterator;
		switch (message->_messageType) {
		case Message::ATTACKED_BY: {
			AttackedByMessage* abMessage = static_cast<AttackedByMessage*>(message);
			std::cout << "Tower at" << _tower->getPosition().x << ", " << _tower->getPosition().z << " is attacked by Target " << abMessage->_attackingEnemyID;
			break;
		}
		case Message::DETONATION_SOUND: {
			DetonationSoundMessage* dsMessage = static_cast<DetonationSoundMessage*>(message);
			if (Utils::distance(_tower->getPosition(), dsMessage->_detonationPoint) < dsMessage->_detonationStrength) {
				std::cout << "Tower at" << _tower->getPosition().x << ", " << _tower->getPosition().z << " heard some detonation at point " << dsMessage->_detonationPoint.x << "," << dsMessage->_detonationPoint.z;
			}
			break;
		}
		}
	}
}

void TowerAI::explore() {

	std::vector<Target*> targets = _scene.getTargets();
	for (std::vector<Target*>::iterator targetsIter = targets.begin(); targetsIter != targets.end(); targetsIter++) {
		Target* target = *targetsIter;
		if(Utils::distance(_tower->getPosition(),target->getPosition()) < SMALLTOWER_VISION_DISTANCE)
		{
			_currentTarget = target;
			break;
		}
	}
	//as long as we dont have a target, do some searching animation
	if(_currentTarget == NULL)
	{
		_tower->setAzimuth(_tower->getAzimuth() + 5);
	}

}

void TowerAI::hunt() {

}

///////////////////////
//helper methods
//////////////////////

void TowerAI::aimAndFire(int target) {

}

}
