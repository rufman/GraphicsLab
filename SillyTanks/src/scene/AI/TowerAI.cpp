/*
 * TowerAI.cpp
 */

#include "TowerAI.hpp"

namespace game_space {

TowerAI::TowerAI(Scene &scene, std::vector<Message*>* aiMessages) :
				_strategy(TowerAI::EXPLORE), _aiMessages(aiMessages), _currentTarget(-1), _scene(scene) {


}

TowerAI::~TowerAI() {

}

void TowerAI::brainTick(){

}

void TowerAI::switchStrategy(enum TOWERAI_STRATEGY newStrategy, int target) {
	_strategy = newStrategy;
	_currentTarget = target;
}

int TowerAI::pickTarget() {
	return _currentTarget;
}

void TowerAI::sense() {
	for (std::vector<Message*>::iterator messageIterator = _aiMessages->begin(); messageIterator != _aiMessages->end(); messageIterator++) {
		Message* message = *messageIterator;
		switch (message->_messageType) {
		case Message::ATTACKED_BY: {
			AttackedByMessage* abMessage = static_cast<AttackedByMessage*>(message);
			std::cout << "Tower " << _tank->getID() << " is attacked by Tank " << abMessage->_attackingEnemyID;
			break;
		}
		case Message::DETONATION_SOUND: {
			DetonationSoundMessage* dsMessage = static_cast<DetonationSoundMessage*>(message);
			if (Utils::distance(_tank->getPosition(), dsMessage->_detonationPoint) < dsMessage->_detonationStrength) {
				std::cout << "Tank " << _tank->getID() << " heard some detonation at point " << dsMessage->_detonationPoint.x << "," << dsMessage->_detonationPoint.z;
			}
			break;
		}
		}
	}
}

void TowerAI::explore() {


}

void TowerAI::hunt() {

}

///////////////////////
//helper methods
//////////////////////

void TowerAI::aimAndFire(int target) {

}

}
