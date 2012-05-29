/*
 * AttackedByMessage.h
 */

#ifndef ATTACKEDBYMESSAGE_H_
#define ATTACKEDBYMESSAGE_H_

#include "Message.hpp"

namespace game_space {

class Target;

class AttackedByMessage: public Message {
public:
	Target* _attackingEnemy;
public:
	AttackedByMessage(Target* attackingEnemy);
	virtual ~AttackedByMessage();
};

} /* namespace game_space */
#endif /* ATTACKEDBYMESSAGE_H_ */
