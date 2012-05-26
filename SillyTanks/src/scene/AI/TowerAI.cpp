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
		_strategy(TowerAI::EXPLORE), _aiMessages(aiMessages), _currentTarget(
				NULL), _scene(scene) {

}

TowerAI::~TowerAI() {

}

void TowerAI::brainTick(float seconds) {

	reloadTime -= seconds;

	//listen closely and watch out for enemies.
	sense();

	//take a decision
	if (_currentTarget == NULL && _strategy != EXPLORE) {
		switchStrategy(EXPLORE, NULL);
	} else if (_currentTarget != NULL && _strategy != HUNT) {
		switchStrategy(HUNT, _currentTarget);
	}

	//do what you decided to do
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

void TowerAI::switchStrategy(enum TOWERAI_STRATEGY newStrategy,
		Target* target) {
	_strategy = newStrategy;
	_currentTarget = target;
}

void TowerAI::sense() {
	for (std::vector<Message*>::iterator messageIterator = _aiMessages->begin();
			messageIterator != _aiMessages->end(); messageIterator++) {
		Message* message = *messageIterator;
		switch (message->_messageType) {
		case Message::ATTACKED_BY: {
			AttackedByMessage* abMessage =
					static_cast<AttackedByMessage*>(message);
			std::cout << "Tower at" << _tower->getPosition().x << ", "
					<< _tower->getPosition().z << " is attacked by Target "
					<< abMessage->_attackingEnemyID;
			break;
		}
		case Message::DETONATION_SOUND: {
			DetonationSoundMessage* dsMessage =
					static_cast<DetonationSoundMessage*>(message);
			if (Utils::distance(_tower->getPosition(),
					dsMessage->_detonationPoint)
					< dsMessage->_detonationStrength) {
				std::cout << "Tower at" << _tower->getPosition().x << ", "
						<< _tower->getPosition().z
						<< " heard some detonation at point "
						<< dsMessage->_detonationPoint.x << ","
						<< dsMessage->_detonationPoint.z;
			}
			break;
		}
		}
	}

	//see
	if (_currentTarget == NULL) {
		std::vector<Target*> targets = _scene.getTargets();
		for (std::vector<Target*>::iterator targetsIter = targets.begin();
				targetsIter != targets.end(); targetsIter++) {
			Target* target = *targetsIter;

			//if the target near you is not yourself and it is near enough that you can see it
			if (target != _tower&& Utils::distance(_tower->getPosition(),
					target->getPosition()) < SMALLTOWER_VISION_DISTANCE) {_currentTarget = target;
		}
			break;
		}
	}
}

void TowerAI::explore() {

	std::vector<Target*> targets = _scene.getTargets();
	for (std::vector<Target*>::iterator targetsIter = targets.begin();
			targetsIter != targets.end(); targetsIter++) {
		Target* target = *targetsIter;
		if (Utils::distance(_tower->getPosition(),
				target->getPosition()) < SMALLTOWER_VISION_DISTANCE) {
			_currentTarget = target;
			break;
		}
	}
	//as long as we dont have a target, do some searching animation
	if (_currentTarget == NULL) {
		_tower->setAzimuth(_tower->getAzimuth() + 5);
	}

}

void TowerAI::hunt() {
	if (reloadTime < 0) {
		aimAndFire();
		reloadTime = SMALLTANK_RELOADING_TIME;
	}
}

///////////////////////
//helper methods
//////////////////////
void TowerAI::aimAndFire() {
	if (_currentTarget != NULL) {
		//get the direction of the enemy tank and point towards it (aim azimuth)
		Vector3D enemyDirection;
		enemyDirection.x = _currentTarget->getPosition().x
				- _tower->getPosition().x;
		enemyDirection.y = _currentTarget->getPosition().y
				- _tower->getPosition().y;
		enemyDirection.z = _currentTarget->getPosition().z
				- _tower->getPosition().z;

		Vector3D muzzleDirection = Vector3D(1, 1, 0);
		Utils::rotate(-_tower->getAzimuth(), muzzleDirection,
				Vector3D(0, 0, 1));
		_tower->setAzimuth(
				Utils::toDegree(Utils::dot(muzzleDirection, enemyDirection)));
		/*
		 //get the elevation
		 _tower->setElevation(
		 Utils::toDegree(Utils::getElevation(_tower->getPosition(),
		 _currentTarget->getPosition(),
		 _tower->getShootingPower())));*/

		_tower->setElevation(45);
		//shoot
		_tower->fireMissile(_currentTarget->getPosition());
	}
}

}
