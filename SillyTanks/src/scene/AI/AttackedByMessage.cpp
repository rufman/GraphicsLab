/*
 * AttackedByMessage.cpp
 * This message is sent to the AI if some enemy attacks the AI controlled tank.
 */

#include "AttackedByMessage.hpp"

namespace game_space {

AttackedByMessage::AttackedByMessage(int attackingEnemyID) :
		Message(Message::ATTACKED_BY), _attackingEnemyID(attackingEnemyID) {
}

AttackedByMessage::~AttackedByMessage() {

}

} /* namespace game_space */
