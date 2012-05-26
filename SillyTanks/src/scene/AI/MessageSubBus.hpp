/*
 * MessageSubBus.hpp
 */

#ifndef MESSAGESUBBUS_H_
#define MESSAGESUBBUS_H_

#include <vector>

namespace game_space {

class Message;
class Target;

class MessageSubBus {
public:

	std::vector<Message*> _messageSubBus;
	Target* _owningTarget;
	MessageSubBus();
	virtual ~MessageSubBus();
};

}

#endif /* MESSAGESUBBUS_H_ */
