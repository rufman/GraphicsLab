/*
 * MessageBus.hpp
 */

#ifndef MESSAGEBUS_H_
#define MESSAGEBUS_H_

#include "Message.hpp"
#include <vector>

namespace game_space {

class MessageBus {
public:
	std::vector<std::vector<Message>*> messageBus;
public:
	MessageBus();
	virtual ~MessageBus();

	int addNewClient();
	std::vector<int>* getMessagesForClient(int id);
};

}
#endif /* MESSAGEBUS_H_ */
