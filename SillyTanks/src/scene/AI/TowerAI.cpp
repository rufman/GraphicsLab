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

void TowerAI::brainTick() {
	sense();
	// if pickTarget is null, sensing failed and we have to explore further
	if (pickTarget() == NULL) {
		switchStrategy(EXPLORE, NULL);
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

Target* TowerAI::pickTarget() {
	return _currentTarget;
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

	/*for (std::vector<Target*>::iterator targetsIter = _scene.getTargets().begin(); targetsIter != _scene.getTargets().end(); targetsIter++) {
		Target* target = *targetsIter;
		if	(Utils::distance(_tower->getPosition(),target->getPosition()) < 10)
		{
			_currentTarget = target;
			break;
		}
	}*/
	if(_currentTarget != NULL)
	{
		//point towards enemy
	}
	else
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
