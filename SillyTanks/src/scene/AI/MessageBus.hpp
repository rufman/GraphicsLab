/*
 * MessageBus.hpp
 */

#ifndef MESSAGEBUS_H_
#define MESSAGEBUS_H_

#include <vector>

namespace game_space {

class MessageSubBus;
class Message;
class Target;

class MessageBus {
public:
	std::vector<MessageSubBus*> messageBus;
public:
	MessageBus();
	virtual ~MessageBus();

	MessageSubBus* addNewClient();
	MessageSubBus* getSubbusOfClient(Target* target);
	void sendMessageTo(Message message,Target* target);
};

}
#endif /* MESSAGEBUS_H_ */
