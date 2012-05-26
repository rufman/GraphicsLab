/*
 * MessageBus.cpp
 */

#include "MessageBus.hpp"

//AI includes
#include "Message.hpp"
#include "MessageSubBus.hpp"

#include "../entities/collisiondetection/Target.hpp"

namespace game_space {

MessageBus::MessageBus() {
}

MessageBus::~MessageBus() {
	//delete the message bus
	for (std::vector<MessageSubBus*>::iterator subBusIter = messageBus.begin();
			subBusIter != messageBus.end();) {
		MessageSubBus* subBus = *subBusIter;
		for (std::vector<Message*>::iterator messageIter =
				subBus->_messageSubBus.begin();
				messageIter != subBus->_messageSubBus.end();) {
			Message* message = *messageIter;
			messageIter = subBus->_messageSubBus.erase(messageIter);
			delete message;
		}
		subBusIter = messageBus.erase(subBusIter);
		delete subBus;
	}
}

MessageSubBus* MessageBus::addNewClient() {
	MessageSubBus* subBus = new MessageSubBus();
	messageBus.push_back(subBus);
	//return the messagebus client id
	return subBus;
}

MessageSubBus* MessageBus::getSubbusOfClient(Target* target) {
	for (std::vector<MessageSubBus*>::iterator subBusIter = messageBus.begin();subBusIter != messageBus.end();) {
		MessageSubBus* subBus = *subBusIter;
		if (subBus->_owningTarget == target) {
			return subBus;
		}
	}
	return NULL;
}

void MessageBus::sendMessageTo(Message message, Target* target) {
	for (std::vector<MessageSubBus*>::iterator subBusIter = messageBus.begin();subBusIter != messageBus.end();subBusIter++) {
		MessageSubBus* subBus = *subBusIter;
		if (subBus->_owningTarget == target) {
			subBus->_messageSubBus.push_back(&message);
			break;
		}
	}
}

}
