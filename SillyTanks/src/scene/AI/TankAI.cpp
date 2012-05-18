/*
 * TankAI.cpp
 *
 */

#include "TankAI.hpp"
#include "../../common/Utils.hpp"
#include "../../common/Types.hpp"
#include <iostream>

#include "DetonationSoundMessage.hpp"
#include "AttackedByMessage.hpp"

namespace game_space {

TankAI::TankAI(Tank* tank, std::vector<Message*>* aiMessages) :
		_strategy(TankAI::EXPLORE), _aiMessages(aiMessages), _tank(tank) {

}

TankAI::~TankAI() {
}

void TankAI::brainTick() {

	sense();
	// if pickTarget is null, sensing failed and we have to explore further
	if (pickTarget() == NULL) {
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
	case RAMPAGE:{
		rampage();
		break;
	}
	}

}

void TankAI::switchStrategy(enum TANKAI_STRATEGY newStrategy, int target) {

}

int TankAI::pickTarget() {
	return -1;
}

void TankAI::sense() {
	for (std::vector<Message*>::iterator messageIterator = _aiMessages->begin(); messageIterator != _aiMessages->end(); messageIterator++) {
		Message* message = *messageIterator;
		switch (message->_messageType) {
		case Message::ATTACKED_BY: {
			AttackedByMessage* abMessage = static_cast<AttackedByMessage*>(message);
			std::cout << "Tank " << _tank->getID() << " is attacked by some other tank.";
			break;
		}
		case Message::DETONATION_SOUND: {
			DetonationSoundMessage* dsMessage = static_cast<DetonationSoundMessage*>(message);
			if (Utils::distance(_tank->getPosition(), dsMessage->_detonationPoint) <dsMessage->_detonationStrength) {
				std::cout << "Tank " << _tank->getID() << " heard some detonation!";
			}
			break;
		}
		}
	}
}

void TankAI::explore() {

}

void TankAI::hunt() {

}

void TankAI::reinforcement() {

}

void TankAI::rampage() {

}

void TankAI::escape() {

}
///////////////////////
//helper methods
//////////////////////

void TankAI::followPath() {

}

void TankAI::aimAndFire(int target) {

}

}
