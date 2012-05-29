/*
 * AttackedByMessage.cpp
 * This message is sent to the AI if some enemy attacks the AI controlled tank.
 */

#include "AttackedByMessage.hpp"
#include "../entities/collisiondetection/Target.hpp"

namespace game_space {

AttackedByMessage::AttackedByMessage(Target* attackingEnemy) :
		Message(Message::ATTACKED_BY), _attackingEnemy(attackingEnemy) {
}

AttackedByMessage::~AttackedByMessage() {

}

} /* namespace game_space */
