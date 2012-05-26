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
		_scene(scene), _strategy(TankAI::EXPLORE), _aiMessages(aiMessages), _currentTarget(
				NULL), _path(NULL) {

}

TankAI::~TankAI() {
	if (_path != NULL) {
		delete _path;
	}
}

void TankAI::brainTick(float seconds) {

	reloadTime -= seconds;

	//listen closely and watch out for enemies.
	sense();

	//take a decision
	if (_currentTarget == NULL && _strategy != EXPLORE) {
		switchStrategy(EXPLORE, NULL);
	} else if(_currentTarget != NULL && _strategy != HUNT) {
		switchStrategy(HUNT, _currentTarget);
		delete _path;
		_path = NULL;
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
	case ESCAPE: {
		escape();
		break;
	}
	}

}

void TankAI::switchStrategy(enum TANKAI_STRATEGY newStrategy, Target* target) {
	_strategy = newStrategy;
	_currentTarget = target;
}

void TankAI::sense() {

	//listen
	for (std::vector<Message*>::iterator messageIterator = _aiMessages->begin();
			messageIterator != _aiMessages->end(); messageIterator++) {
		Message* message = *messageIterator;
		switch (message->_messageType) {
		case Message::ATTACKED_BY: {
			AttackedByMessage* abMessage =
					static_cast<AttackedByMessage*>(message);
			std::cout << "Tank " << " is attacked by Tank "
					<< abMessage->_attackingEnemyID;
			switchStrategy(ESCAPE, _currentTarget);
			break;
		}
		case Message::DETONATION_SOUND: {
			DetonationSoundMessage* dsMessage =
					static_cast<DetonationSoundMessage*>(message);
			if (Utils::distance(_tank->getPosition(),
					dsMessage->_detonationPoint)
					< dsMessage->_detonationStrength) {

				std::cout << "Tank " << " heard some detonation at point "
						<< dsMessage->_detonationPoint.x << ","
						<< dsMessage->_detonationPoint.z;

				//if the tank heard a detonation it goes and checks out who's there
				if (_path == NULL) {
					delete _path;
					_path = NULL;
				}
				_path = _scene.getTerrain().findPath(_tank->getPosition(),
						dsMessage->_detonationPoint);
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
			if (target != _tank&& Utils::distance(_tank->getPosition(),
					target->getPosition()) < SMALLTANK_VISION_DISTANCE) {_currentTarget = target;
			break;
		}
	}
} else {
	//if the target gets out of sight, we lose it
	if (Utils::distance(_tank->getPosition(),
					_currentTarget->getPosition()) > SMALLTANK_VISION_DISTANCE) {
		_currentTarget = NULL;
	}
}
}

void TankAI::explore() {
	//choose some random position on the map and find a way from here to this position
	while (_path == NULL) {
		Point randomGoal = _scene.getTerrain().getRandomPointOnMap();
		_path = _scene.getTerrain().findPath(_tank->getPosition(), randomGoal);
	}

	followPath();

	//as long as we dont have a target, do some searching animation
	_tank->setAzimuth(_tank->getAzimuth() + 5);
}

void TankAI::hunt() {
	//choose some random position on the map and find a way from here to this position
	if(Utils::distance(_tank->getPosition(),_currentTarget->getPosition()) > SHOOTING_DISTANCE)
	{
		while (_path == NULL) {
			Point randomGoal;
			do
			{
				randomGoal = _scene.getTerrain().getRandomPointOnMap();
			}while(Utils::distance(randomGoal,_currentTarget->getPosition()) < SHOOTING_DISTANCE);
			_path = _scene.getTerrain().findPath(_tank->getPosition(), randomGoal);
		}

		followPath();
	}

	if (reloadTime < 0) {
		aimAndFire();
		reloadTime = SMALLTANK_RELOADING_TIME;
	}
}

void TankAI::escape() {

	//choose some random position on the map and find a way from here to this position
	while (_path == NULL) {
		Point randomGoal;
		do {
			randomGoal = _scene.getTerrain().getRandomPointOnMap();
		} while (Utils::distance(_currentTarget->getPosition(), randomGoal)
				> SMALLTANK_ESPACE_DISTANCE);
		_path = _scene.getTerrain().findPath(_tank->getPosition(), randomGoal);
	}

	followPath();

}
///////////////////////
//helper methods
//////////////////////

void TankAI::followPath() {
	Point nextCheckPoint;
	for (std::vector<Point>::iterator pathIter = _path->begin();
			pathIter != _path->end(); pathIter++) {
		nextCheckPoint = *pathIter;
		if (Utils::distance(_tank->getPosition(), nextCheckPoint) > 1) {
			// the currently chosen point is the next we should try to get to.
			break;
		} else {
			//we already reached that point
			_path->pop_back();
		}
	}

	if (_path->size() == 0) {
		delete _path;
		_path = NULL;
		return;
	}

	Vector3D directionToCheckPoint;
	directionToCheckPoint.x = nextCheckPoint.x - _tank->getPosition().x;
	directionToCheckPoint.y = nextCheckPoint.y - _tank->getPosition().y;
	directionToCheckPoint.z = nextCheckPoint.z - _tank->getPosition().z;

	Utils::normalize(directionToCheckPoint);
	Vector3D tankDirection = Utils::rotate(_tank->getDirection(),
			Vector3D(0, 0, 1), Vector3D(0, 1, 0));
	float rotationAngle = Utils::dot(tankDirection, directionToCheckPoint);
	if (rotationAngle != 0) {
		_tank->setDirection(
				_tank->getDirection() + Utils::toDegree(rotationAngle));
	}
	_tank->move(SMALLTANK_SPEED);
}

void TankAI::aimAndFire() {
	if (_currentTarget != NULL) {
		//get the direction of the enemy tank and point towards it (aim azimuth)

		Vector3D enemyDirection;
		enemyDirection.x = _currentTarget->getPosition().x
				- _tank->getPosition().x;
		enemyDirection.y = _currentTarget->getPosition().y
				- _tank->getPosition().y;
		enemyDirection.z = _currentTarget->getPosition().z
				- _tank->getPosition().z;

		Vector3D muzzleDirection = Vector3D(1, 0, 0);
		Utils::rotate(_tank->getAzimuth(), muzzleDirection, Vector3D(0, 1, 0));
		_tank->setAzimuth(Utils::dot(muzzleDirection, enemyDirection));

		//get the elevation
		_tank->setElevation(
				Utils::getElevation(_tank->getPosition(),
						_currentTarget->getPosition(),
						_tank->getShootingPower()));
		//shoot
		_tank->fireBullet();
	}
}
}
