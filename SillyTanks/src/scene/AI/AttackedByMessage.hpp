/*
 * AttackedByMessage.h
 */

#ifndef ATTACKEDBYMESSAGE_H_
#define ATTACKEDBYMESSAGE_H_

#include "Message.hpp"

namespace game_space {

class AttackedByMessage: public Message {
public:
	int _attackingEnemyID;
public:
	AttackedByMessage(int attackingEnemyID);
	virtual ~AttackedByMessage();
};

} /* namespace game_space */
#endif /* ATTACKEDBYMESSAGE_H_ */
