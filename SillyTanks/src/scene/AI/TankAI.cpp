/*
 * TankAI.cpp
 *
 */

#include "TankAI.hpp"
#include "../../common/Utils.hpp"
#include "../../common/Types.hpp"
#include "../Terrain.hpp"
#include <iostream>

#include "DetonationSoundMessage.hpp"
#include "AttackedByMessage.hpp"

namespace game_space {

TankAI::TankAI(Scene &scene, std::vector<Message*>* aiMessages) :
		_strategy(TankAI::EXPLORE), _aiMessages(aiMessages), _currentTarget(NULL), _scene(scene) {

}

TankAI::~TankAI() {
}

void TankAI::brainTick(float seconds) {

	//TODO:add reloadingtime

	sense();
	// if pickTarget is null, sensing failed and we have to explore further
	if (pickTarget() == -1) {
		switchStrategy(EXPLORE, -1);
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
	case ESCAPE: {
		escape();
		break;
	}
	}

}

void TankAI::switchStrategy(enum TANKAI_STRATEGY newStrategy, int target) {
	_strategy = newStrategy;
	_currentTarget = target;
}

int TankAI::pickTarget() {
	return _currentTarget;
}

void TankAI::sense() {
	for (std::vector<Message*>::iterator messageIterator = _aiMessages->begin(); messageIterator != _aiMessages->end(); messageIterator++) {
		Message* message = *messageIterator;
		switch (message->_messageType) {
		case Message::ATTACKED_BY: {
			AttackedByMessage* abMessage = static_cast<AttackedByMessage*>(message);
			std::cout << "Tank " <<  " is attacked by Tank " << abMessage->_attackingEnemyID;
			break;
		}
		case Message::DETONATION_SOUND: {
			DetonationSoundMessage* dsMessage = static_cast<DetonationSoundMessage*>(message);
			if (Utils::distance(_tank->getPosition(), dsMessage->_detonationPoint) < dsMessage->_detonationStrength) {
				std::cout << "Tank " << " heard some detonation at point " << dsMessage->_detonationPoint.x << "," << dsMessage->_detonationPoint.z;
			}
			break;
		}
		}
	}
}

void TankAI::explore() {
	//choose some random position on the map and find a way from here to this position
	if (_path == NULL) {
		//Point randomGoal = _scene.getTerrain().getRandomPointOnMap();
		//_path = _scene.getTerrain().findPath(_tank->getPosition(), randomGoal);
	}

	//followPath();

}

void TankAI::hunt() {
}

void TankAI::escape() {

}
///////////////////////
//helper methods
//////////////////////

void TankAI::followPath() {
	Point nextCheckPoint;
	for (std::vector<Point>::iterator pathIter = _path->begin(); pathIter != _path->end(); pathIter++) {
		nextCheckPoint = *pathIter;
		if (Utils::distance(_tank->getPosition(), nextCheckPoint) > 1) {
			// the currently chosen point is the next we should try to get to.
			break;
		} else {
			//we already reached that point
			_path->pop_back();
		}
	}

	Vector3D directionToCheckPoint;
	directionToCheckPoint.x = nextCheckPoint.x - _tank->getPosition().x;
	directionToCheckPoint.y = nextCheckPoint.y - _tank->getPosition().y;
	directionToCheckPoint.z = nextCheckPoint.z - _tank->getPosition().z;

	Utils::normalize(directionToCheckPoint);
	float rotationAngle = Utils::dot(_tank->getDirection(), directionToCheckPoint);
	if (rotationAngle != 0) {
		_tank->setDirection(_tank->getDirection() + Utils::toDegree(rotationAngle));
	}
	_tank->move(SMALLTANK_SPEED);
}

void TankAI::aimAndFire() {
	if(_currentTarget != NULL)
	{
		//get the direction of the enemy tank and point towards it (aim azimuth)
		Vector3D enemyDirection = _currentTarget->getPosition()- _tank->getPosition();
		Vector3D muzzleDirection = Vector3D(0,0,1);
		Utils::rotate(_tank->getAzimuth(),muzzleDirection,Vector3D(0,1,0));
		_tank->setAzimuth(Utils::dot(muzzleDirection,enemyDirection));

		//get the elevation

		//shoot
		_tank->fireBullet();
	}
}

}
