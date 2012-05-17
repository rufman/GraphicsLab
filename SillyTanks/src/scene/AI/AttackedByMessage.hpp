/*
 * AttackedByMessage.h
 */

#ifndef ATTACKEDBYMESSAGE_H_
#define ATTACKEDBYMESSAGE_H_

#include "Message.hpp"

namespace game_space {

class AttackedByMessage: public Message {
public:
	AttackedByMessage(int enemyTankID);
	virtual ~AttackedByMessage();
};

} /* namespace game_space */
#endif /* ATTACKEDBYMESSAGE_H_ */
